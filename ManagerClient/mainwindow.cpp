#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initialPage = new InitialPage(this);
	connect(initialPage, SIGNAL(initialFinish(InitialParameters))
			, this, SLOT(initialHandle(InitialParameters)));
	initialPage->show();
	initialPage->setModal(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initialHandle(InitialParameters para) {
	parameters = para;
	QString address = para.address;
	QString port = para.port;
	qDebug()<<address<<"========"<<port;
	socket = new QTcpSocket(this);
	socket->connectToHost(address, port.toInt());

	connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
	connect(socket, SIGNAL(disconnected()), this, SLOT(offLine()));
	connect(&socketConnectTimer, &QTimer::timeout, this, &MainWindow::failToConnectServer);
	socketConnectTimer.setSingleShot(true);
	socketConnectTimer.start(5 * 1000);
}

//连接建立成功
void MainWindow::onConnected() {
	socketConnectTimer.stop();
	connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
	//传到服务器
	QJsonObject ojson;
	using namespace SocketConstants;
	ojson.insert(TYPE, SET_PARA);
	ojson.insert(DEFAULT_TARGET_TEMP, parameters.defaultTargetTemp);
	ojson.insert(DEFAULT_SPEED, parameters.defaultFanSpeed);
	ojson.insert(MAX_TARGET_TEMP, parameters.maxTargetTemp);
	ojson.insert(MIN_TARGET_TEMP, parameters.minTargetTemp);
	ojson.insert(HIGH_FEE_RATE, parameters.highFeeRate);
	ojson.insert(MID_FEE_RATE, parameters.middleFeeRate);
	ojson.insert(LOW_FEE_RATE, parameters.lowFeeRate);
	ojson.insert(WORK_MODE, parameters.mode);
	sendJSON(ojson);
}

//接收服务器消息
void MainWindow::newServerMessage() {
	using namespace SocketConstants;
	while(socket->bytesAvailable() > 0) {
		QByteArray head;
		QByteArray body;
		head = socket->read(HEAD_LEANGTH);
		if(head.size() == 0){
			continue;
		}
		int length;
		memcpy(&length, head.data(), sizeof (length));
		for(int i = 0; i < length; i++)
			body.append(socket->read(1));
		qDebug()<<"收到服务器的信息为："<<body;
		//对body进行处理
		processPacket(body);
	}
}

//处理收到的数据包
void MainWindow::processPacket(QByteArray body){
	using namespace SocketConstants;
	QJsonParseError e;
	QJsonDocument doc = QJsonDocument::fromJson(body, &e);
	if(e.error != QJsonParseError::NoError) {
		qDebug() << "JSON格式错误";
		return;
	}

	QJsonObject ojson = doc.object();
	int type = ojson.value(TYPE).toInt();
	switch (type) {
	case SET_PARA_OK:
	{
		QJsonObject ojson;
		ojson.insert(TYPE, START_UP);
		sendJSON(ojson);
		break;
	}
	case START_UP_OK:
	{
		qDebug()<<"start up ok!";
		connect(&monitorTimer, &QTimer::timeout, this, &MainWindow::onMonitor);
		monitorTimer.start(monitorInterval);
		break;
	}
	case CHECK_ROOM_STATE_OK:
	{
		for(int i=1; i < 6; i++){
			QJsonObject t;
			t = ojson.value(QString(i)).toObject();
			QString display;
			display = parseRoomState(t);
			switch(i) {
			case 1:
				this->ui->textEdit_1->setText(display);
				break;
			case 2:
				this->ui->textEdit_2->setText(display);
				break;
			case 3:
				this->ui->textEdit_3->setText(display);
				break;
			case 4:
				this->ui->textEdit_4->setText(display);
				break;
			case 5:
				this->ui->textEdit_5->setText(display);
				break;
			}
		}
		break;
	}
	case CREATE_INVOICE_OK:
	{
		Invoice invoice;
		invoice.roomId = ojson.value(ROOM_ID).toInt();
		invoice.totalFee = ojson.value(TOTAL_FEE).toDouble();
		curInvoice = invoice;
		if(invoicePage != nullptr)
			delete invoicePage;
		invoicePage = new InvoicePage(this, invoice);
		connect(invoicePage, &InvoicePage::PrintInvoice,
				this, &MainWindow::onPrintInvoice);
		connect(invoicePage, &InvoicePage::PrintDR,
				this, &MainWindow::onCreateDR);
		invoicePage->show();
		break;
	}
	case CREATE_DR_OK:
	{
		print_DR(ojson);
		break;
	}
	case QUERY_REPORT_OK:
	{
		printReport(ojson);
		break;
	}
	}
}

//打印报表
void MainWindow::printReport(QJsonObject ojson){
	using namespace SocketConstants;
	Report r;
	r.date = ojson.value(DATE).toString();
	QJsonArray array = ojson.value(REPORT_ARRAY).toArray();
	for(const auto &item : array){
		QJsonObject tjson = item.toObject();
		struct report t;
		t.RoomId = tjson.value(ROOM_ID).toInt();
		t.SwitchOnoffTime = tjson.value(SWITCH_ON_OFF_TIMES).toInt();
		t.ServiceTime = tjson.value(SERVICE_TIME).toDouble();
		t.TotalFee = tjson.value(TOTAL_FEE).toDouble();
		t.ScheduleTime = tjson.value(SCHEDULE_TIME).toInt();
		t.DetailRecordNum = tjson.value(NUM_DR).toInt();
		t.ChangeTempTime = tjson.value(TIMES_CHANGE_TEMP).toInt();
		t.ChangeFanSpeedTime = tjson.value(TIMES_CHANGE_SPEED).toInt();
		r.report.push_back(t);
	}

	r.CreateReportFile();

	ojson.insert(TYPE, PRINT_REPORT);

	sendJSON(ojson);

	QMessageBox::information(this, "成功", "成功生成并打印报表");
}

// 打印详单
void MainWindow::print_DR(QJsonObject ojson){
	using namespace SocketConstants;
	DetailRecords records;
	records.roomId = ojson.value(ROOM_ID).toInt();
	records.totalFee = ojson.value(TOTAL_FEE).toDouble();
	QJsonArray array = ojson.value(DETAIL_ARRAY).toArray();
	QVector<struct DetailRecord> data;
	for(const auto &tvalue : array){
		QJsonObject tjson = tvalue.toObject();
		struct DetailRecord t;
		t.RoomId = tjson.value(ROOM_ID).toInt();
		t.StartTime = tjson.value(START_TIME).toString();
		t.EndTime = tjson.value(END_TIME).toString();
		t.TargetTemp = tjson.value(TARGET_TEMP).toDouble();
		t.FeeRate = tjson.value(FEE_RATE).toDouble();
		t.FanSpeed = tjson.value(FAN_SPEED).toInt();
		t.Duration = tjson.value(DURATION).toDouble();
		t.Fee = tjson.value(CUR_FEE).toDouble();
		data.push_back(t);
	}
	records.detail = data;

	records.PrintAsFile();

	ojson.insert(TYPE, PRINT_DR);

	sendJSON(ojson);

	QMessageBox::information(this, "成功", "成功生成并打印详单");
}

//打印账单
void MainWindow::onPrintInvoice(){
	curInvoice.PrintAsFile();
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, PRINT_INVOICE);
	ojson.insert(ROOM_ID, curInvoice.roomId);
	ojson.insert(TOTAL_FEE, curInvoice.totalFee);
	sendJSON(ojson);
}

//打印详单
void MainWindow::onCreateDR(int RoomId){
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CREATE_DR);
	ojson.insert(ROOM_ID, RoomId);
	sendJSON(ojson);
}

QString MainWindow::parseRoomState(QJsonObject ojson){
	using namespace SocketConstants;
	QString result;

	QString t;
	int state = ojson.value(ROOM_STATE).toInt();
	switch(state) {
	case 0:
		t = QString("空调状态：%1\n").arg("关机");
		break;
	case 1:
		t = QString("空调状态：%1\n").arg("等待");
		break;
	case 2:
		t = QString("空调状态：%1\n").arg("运行");
		break;
	case 3:
		t = QString("空调状态：%1\n").arg("回温");
		break;
	}
	result.append(t);

	double curTemp = ojson.value(CUR_TEMP).toDouble();
	t = QString("当前室温：%1\n").arg(QString::number(curTemp, 'f', 1));
	if(curTemp < 0)
		t = QString("当前室温：%1\n").arg("未知");
	result.append(t);

	double targetTemp = ojson.value(TARGET_TEMP).toDouble();
	t = QString("目标温度：%1\n").arg(QString::number(targetTemp, 'f', 1));
	if(targetTemp < 0)
		t = QString("目标温度：%1\n").arg("未知");
	result.append(t);

	int speed = ojson.value(CUR_SPEED).toInt();
	switch(speed){
	case 0:
		t = QString("当前风速：%1\n").arg("低");
		break;
	case 1:
		t = QString("当前风速：%1\n").arg("中");
		break;
	case 2:
		t = QString("当前风速：%1\n").arg("高");
		break;
	}
	if(speed < 0)
		t = QString("当前风速：%1\n").arg("未知");
	result.append(t);

	double fee = ojson.value(CUR_FEE).toDouble();
	t = QString("当前费用：%1\n").arg(QString::number(fee, 'f', 2));
	if(fee < 0)
		t = QString("当前费用：%1\n").arg("未知");
	result.append(t);

	double totalFee = ojson.value(TOTAL_FEE).toDouble();
	t = QString("累计费用：%1\n").arg(QString::number(totalFee, 'f', 2));
	if(totalFee < 0)
		t = QString("累计费用：%1\n").arg("未知");
	result.append(t);

	return result;
}

// 提出CheckRoomState请求
void MainWindow::onMonitor() {
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHECK_ROOM_STATE);
	sendJSON(ojson);
}

//点击“生成账单”
void MainWindow::on_ptn_makeInvoice_clicked()
{
	queryInvoiceInputDialg = new QueryInvoiceInputDialog(this);
	connect(queryInvoiceInputDialg, &QueryInvoiceInputDialog::queryFinish,
			this, &MainWindow::queryInputFinish);
	queryInvoiceInputDialg->show();
	queryInvoiceInputDialg->setModal(true);
}

//返回Room_Id之后
void MainWindow::queryInputFinish(int Room_Id)
{
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CREATE_INVOICE);
	ojson.insert(ROOM_ID, Room_Id);
	sendJSON(ojson);
}

//点击“生成报表”
void MainWindow::on_ptn_makeReport_clicked()
{
	reportPage = new ReportPage(this);
	connect(reportPage, &ReportPage::Print_Report,
			this, &MainWindow::onPrintReport);
	reportPage->show();
}

//选择完成，打印报表
void MainWindow::onPrintReport(int RoomId, QString date){
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, QUERY_REPORT);
	ojson.insert(ROOM_ID, RoomId);
	ojson.insert(DATE, date);
	sendJSON(ojson);
}

void MainWindow::sendJSON(QJsonObject ojson) {
	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	sendPacket(msg);
}

void MainWindow::sendPacket(QByteArray body) {
	QByteArray head;
	// 构造头部
	int  length = body.size();
	int len_int = sizeof(length);
	head.resize(len_int);
	memcpy(head.data(), &length, len_int);
	QByteArray packet;
	packet = head + body;
	socket->write(packet, packet.size());
}

//断线检测
void MainWindow::offLine(){
	QString content("与服务器断开连接，请检查您的网络");
	msgBox = new QMessageBox(nullptr);
	msgBox->setWindowTitle(QString("ERROR"));
	msgBox->setText(content);
	msgBox->setModal(true);
	msgBox->addButton(QMessageBox::Yes);
	msgBox->setIcon(QMessageBox::Critical);
	connect(msgBox, &QMessageBox::destroyed, [=](){
		this->destroy();
	});
	connect(msgBox, &QMessageBox::buttonClicked,
			[=](){
		delete msgBox;
	});
	msgBox->show();
}

//与服务器建立连接失败
void MainWindow::failToConnectServer(){
	QString content("无法与服务器建立连接，请检查您的网络");
	msgBox = new QMessageBox(nullptr);
	msgBox->setWindowTitle(QString("ERROR"));
	msgBox->setText(content);
	msgBox->setModal(true);
	msgBox->addButton(QMessageBox::Yes);
	msgBox->setIcon(QMessageBox::Critical);
	connect(msgBox, &QMessageBox::destroyed, [=](){
		this->destroy();
	});
	connect(msgBox, &QMessageBox::buttonClicked,
			[=](){
		delete msgBox;
	});
	msgBox->show();
}
