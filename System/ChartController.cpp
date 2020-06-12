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
	}

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
