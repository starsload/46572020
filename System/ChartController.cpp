#include "ChartController.h"
#include "AirConditionHost.h"

ChartController::ChartController(QObject *parent):
	QObject(parent)
{
}

void ChartController::setAirConditionHostRelation(AirConditionHost *host){
	airConditionHost = host;
}

void ChartController::setMonitorRelation(Monitor *m){
	monitor = m;
}

void ChartController::setSocket(QTcpSocket *s) {
	socket = s;
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(listenToManagerClient()));
}

void ChartController::listenToManagerClient(){
	qDebug()<<"收到新消息ManagerClient，地址为："<<socket->peerAddress()
		  <<"端口为："<<socket->peerPort();
	using namespace SocketConstants;
	while(socket->bytesAvailable() > 0) {
		QByteArray head;
		QByteArray body;
		socket->waitForReadyRead();
		head = socket->read(HEAD_LEANGTH);
		if(head.size() == 0){
			continue;
		}
		int length;
		memcpy(&length, head.data(), sizeof (length));
		for(int i = 0; i < length; i++)
			body.append(socket->read(1));
		qDebug()<<"收到的消息为：\n"<<body;
		processPacket(body);
	}
}

//对收到的数据包进行处理
void ChartController::processPacket(QByteArray body){
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
	case SET_PARA:
	{
		int defaultTargetTemp = ojson.value(DEFAULT_TARGET_TEMP).toInt();
		int maxTargetTemp = ojson.value(MAX_TARGET_TEMP).toInt();
		int minTargetTemp = ojson.value(MIN_TARGET_TEMP).toInt();
		double highFeeRate = ojson.value(HIGH_FEE_RATE).toDouble();
		double middleFeeRate = ojson.value(MID_FEE_RATE).toDouble();
		double lowFeeRate = ojson.value(LOW_FEE_RATE).toDouble();
		int mode = ojson.value(WORK_MODE).toInt();
		int speed = ojson.value(DEFAULT_SPEED).toInt();
		setPara(defaultTargetTemp, maxTargetTemp, minTargetTemp,
				highFeeRate, middleFeeRate, lowFeeRate, mode, speed);
		break;
	}
	case START_UP:
	{
		startUp();
		break;
	}
	case CHECK_ROOM_STATE:
	{
		CheckRoomState();
		break;
	}
	case CREATE_INVOICE:
	{
		int id = ojson.value(ROOM_ID).toInt();
		CreateInvoice(id);
		break;
	}
	case PRINT_INVOICE:
	{
		Invoice invoice;
		invoice.roomId = ojson.value(ROOM_ID).toInt();
		invoice.totalFee = ojson.value(TOTAL_FEE).toDouble();
		PrintInvoice(invoice);
		break;
	}
	case CREATE_DR:
	{
		int id = ojson.value(ROOM_ID).toInt();
		CreateRDR(id);
		break;
	}
	case PRINT_DR:
	{
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

		PrintRDR(records);
		break;
	}
	case QUERY_REPORT:
	{
		int id = ojson.value(ROOM_ID).toInt();
		QString date = ojson.value(DATE).toString();
		QueryReport(id, date);
		break;
	}
	case PRINT_REPORT:
	{
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

		PrintReport(r);
		break;
	}
	}
}

void ChartController::PrintReport(const Report &r){
	airConditionHost->RequestPrintReport(r);
}

void ChartController::QueryReport(int RoomId, QString date){
	Report r;
	r = airConditionHost->RequestReport(date);

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, QUERY_REPORT_OK);
	ojson.insert(DATE, r.date);
	QJsonArray array;
	for(const auto &item : r.report) {
		QJsonObject t;
		t.insert(ROOM_ID, item.RoomId);
		t.insert(SWITCH_ON_OFF_TIMES, item.SwitchOnoffTime);
		t.insert(SERVICE_TIME, item.ServiceTime);
		t.insert(TOTAL_FEE, item.TotalFee);
		t.insert(SCHEDULE_TIME, item.ScheduleTime);
		t.insert(NUM_DR, item.DetailRecordNum);
		t.insert(TIMES_CHANGE_TEMP, item.ChangeTempTime);
		t.insert(TIMES_CHANGE_SPEED, item.ChangeFanSpeedTime);
		array.push_back(t);
	}
	ojson.insert(REPORT_ARRAY, array);
	sendJSON(ojson);
}

void ChartController::PrintRDR(DetailRecords d){
	d.PrintAsFile();
}

void ChartController::CreateRDR(int RoomId){
	DetailRecords detailRecords;
	detailRecords = airConditionHost->MakeDR(RoomId);

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CREATE_DR_OK);
	ojson.insert(ROOM_ID, detailRecords.roomId);
	ojson.insert(TOTAL_FEE, detailRecords.totalFee);
	QJsonArray array;
	for(const auto &item : detailRecords.detail){
		QJsonObject t;
		t.insert(ROOM_ID, item.RoomId);
		t.insert(START_TIME, item.StartTime);
		t.insert(END_TIME, item.EndTime);
		t.insert(TARGET_TEMP, item.TargetTemp);
		t.insert(FAN_SPEED, item.FanSpeed);
		t.insert(FEE_RATE, item.FeeRate);
		t.insert(DURATION, item.Duration);
		t.insert(CUR_FEE, item.Fee);
		array.push_back(t);
	}
	ojson.insert(DETAIL_ARRAY, array);
	sendJSON(ojson);
}

void ChartController::CreateInvoice(int Room_Id){
	Invoice invoice;
	invoice = airConditionHost->MakeAnInvoice(Room_Id);
	qDebug() << QString("管理员创建%1号房间的账单成功").arg(Room_Id);

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CREATE_INVOICE_OK);
	ojson.insert(ROOM_ID, invoice.roomId);
	ojson.insert(TOTAL_FEE, invoice.totalFee);
	sendJSON(ojson);
}

void ChartController::PrintInvoice(Invoice v){
	v.PrintAsFile();
}

void ChartController::setPara(int defaultTargetTemp, int maxTargetTemp, int minTargetTemp,
double highFeeRate, double middleFeeRate, double lowFeeRate, int mode, int speed){
	airConditionHost->setPara(defaultTargetTemp, maxTargetTemp, minTargetTemp,
							  highFeeRate, middleFeeRate, lowFeeRate, mode, speed);
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, SET_PARA_OK);
	sendJSON(ojson);
}

void ChartController::startUp(){
	airConditionHost->startUp();
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, START_UP_OK);
	sendJSON(ojson);
}

void ChartController::CheckRoomState(){
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHECK_ROOM_STATE_OK);
	for(int i=1;i<6;i++){
		AirConditionClient *client;
		QJsonObject tjson;
		client = monitor->SetupMonitor(i);
		if (client == nullptr){
			tjson.insert(ROOM_STATE, 0);
			tjson.insert(CUR_TEMP, -1);
			tjson.insert(TARGET_TEMP, -1);
			tjson.insert(CUR_SPEED, -1);
			tjson.insert(CUR_FEE, -1);
			tjson.insert(TOTAL_FEE, -1);
		}
		else{
			int state;
			state = client->GetState();
			if(state == 0)
				state = 3;
			double curTemp;
			curTemp = client->GetPreTemp();
			double targetTemp;
			targetTemp = client->GetTargetTemp();
			int speed;
			speed = client->GetFanSpeed();
			double fee;
			fee = client->GetFee();
			double totalFee;
			totalFee = client->GetTotalFee();

			tjson.insert(ROOM_STATE, state);
			tjson.insert(CUR_TEMP, curTemp);
			tjson.insert(TARGET_TEMP, targetTemp);
			tjson.insert(CUR_SPEED, speed);
			tjson.insert(CUR_FEE, fee);
			tjson.insert(TOTAL_FEE, totalFee);
		}
		ojson.insert(QString(i), tjson);
	}
	sendJSON(ojson);
}

void ChartController::sendJSON(QJsonObject ojson){
	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	sendPacket(msg);
}

void ChartController::sendPacket(QByteArray body){
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

void ChartController::RequestOff(int RoomId){
   // airConditionHost->TurnOff(RoomId);
}
