#include "ScheduleController.h"
#include "AirConditionHost.h"

ScheduleController::ScheduleController(QObject *parent):
	QObject(parent)
{

}

ScheduleController::~ScheduleController()
{
}

void ScheduleController::setAirConditionHost(AirConditionHost* s){
	airConditionHost = s;
}

void ScheduleController::setMonitorRelation(Monitor *m){
	monitor = m;
}

void ScheduleController::addGuestSocket(QTcpSocket *s){
	GuestClientSocket *mySocket;
	mySocket = new GuestClientSocket(s);
	qDebug()<<"对端的地址为："<<s->peerAddress()<<"端口为"<<s->peerPort();
	allSockets.push_back(mySocket);

	connect(mySocket, SIGNAL(newPacketArrive(QTcpSocket*)),
			this, SLOT(listenToGuestClient(QTcpSocket*)));

	connect(mySocket, &GuestClientSocket::Disconnected,
			this, &ScheduleController::onDisconnected);
}

// 如果顾客客户端掉线
void ScheduleController::onDisconnected(QTcpSocket *s){
	QVector<GuestClientSocket*>::iterator iter;
	for(iter=allSockets.begin(); iter != allSockets.end(); iter++){
		GuestClientSocket *sock = *iter;
		if(sock->socket == s){

			airConditionHost->TurnOff(sock->Room_id);

			allSockets.erase(iter);
			delete sock;
			break;
		}
	}
}

void ScheduleController::listenToGuestClient(QTcpSocket *socket){
	for(auto sock : allSockets){
		if(sock->socket == socket){
			curSocket = sock;
		}
	}
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
		QString out = body;
        qDebug()<<"databag：\n"<<out;
		//对body进行处理
		processPacket(body);
	}
}

void ScheduleController::processPacket(QByteArray body){
	using namespace SocketConstants;
	QJsonParseError e;
	QJsonDocument doc = QJsonDocument::fromJson(body, &e);
	if(e.error != QJsonParseError::NoError) {
        qDebug() << "JSON error";
		return;
	}

	QJsonObject ojson = doc.object();
	int type = ojson.value(TYPE).toInt();
	switch (type) {
	case REQUEST_ON:
	{
		int room_id = ojson.value(ROOM_ID).toInt();
		double CurrentRoomTemp = ojson.value(CUR_TEMP).toDouble();
		curSocket->Room_id = room_id;
		RequestOn(room_id, CurrentRoomTemp);
		break;
	}
	case REQUEST_OFF:
	{
		int id = ojson.value(ROOM_ID).toInt();
		RequestOff(id);
		break;
	}
	case CHANGE_FAN_SPEED:
	{
		int id = ojson.value(ROOM_ID).toInt();
		int speed = ojson.value(FAN_SPEED).toInt();
		ChangeFanSpeed(id, speed);
		break;
	}
	case CHANGE_TARGET_TEMP:
	{
		int roomId = ojson.value(ROOM_ID).toInt();
		double targetTemp = ojson.value(TARGET_TEMP).toDouble();
		ChangeTargetTemp(roomId, targetTemp);
		break;
	}
	case REQUEST_SERVICE:
	{
		int id = ojson.value(ROOM_ID).toInt();
		double curTemp = ojson.value(CUR_TEMP).toDouble();
		RequestService(id, curTemp);
		break;
	}
	case REQUEST_FEE:
	{
		int id = ojson.value(ROOM_ID).toInt();
		RequestFee(id);
		break;
	}
	}
}

void ScheduleController::RequestOn(int RoomId,double CurrentRoomTemp){
	returnRequestOn r = airConditionHost->CtreatClient(RoomId, CurrentRoomTemp);
	QJsonObject ojson;
	using namespace SocketConstants;
	ojson.insert(TYPE, REQUEST_ON_OK);
	ojson.insert(ROOM_ID, r.RoomId);
	ojson.insert(CUR_TEMP, r.curTemp);
	ojson.insert(TARGET_TEMP, r.targetTemp);
	ojson.insert(CUR_SPEED, r.curFanSpeed);
	ojson.insert(TOTAL_FEE, r.totalFee);
	ojson.insert(WORK_MODE, r.mode);
	sendJSON(ojson);
}

void ScheduleController::RequestOff(int RoomId){
	airConditionHost->TurnOff(RoomId);
    qDebug()<<QString("%0 room RequestOff complete").arg(RoomId);
}

void ScheduleController::RequestFee(int RoomId){
	ReturnCheckFeeAndTemp result;
	result = monitor->CheckRoomFee(RoomId);
    qDebug()<<QString("%0 room RequestFee complete").arg(RoomId);

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, REQUEST_FEE_OK);
	ojson.insert(CUR_TEMP, result.PreTemp);
	ojson.insert(CUR_FEE, result.Fee);
	ojson.insert(TOTAL_FEE, result.TotalFee);
	sendJSON(ojson);
}

//TODO:补全不能提供服务部分
void ScheduleController::RequestService(int RoomId, float curTemp){
    if(airConditionHost->RequestService(RoomId,curTemp)){//可以提供服务
        using namespace SocketConstants;
        QJsonObject ojson;
        ojson.insert(TYPE, REQUEST_SERVICE_OK);
        sendJSON(ojson);
        qDebug()<<QString("%0 room served").arg(RoomId);
    }
    else {//补全不能提供服务部分
		using namespace SocketConstants;
		QJsonObject ojson;
		ojson.insert(TYPE, REQUEST_SERVICE_FAIL);
		sendJSON(ojson);
        qDebug()<<QString("%0 room wait").arg(RoomId);
    }

}

void ScheduleController::ChangeFanSpeed(int RoomId, int Speed){
	airConditionHost->ChangeFanSpeed(RoomId, Speed);
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHANGE_FAN_SPEED_OK);
	sendJSON(ojson);
    qDebug()<<QString("%0 room change FanSpeed,FanSpeed:%1").arg(RoomId).arg(Speed);
}

void ScheduleController::ChangeTargetTemp(int RoomId, float targetTemp){
	airConditionHost->ChangeTargetTemp(RoomId, targetTemp);
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHANGE_TARGET_TEMP_OK);
	sendJSON(ojson);
    qDebug()<<QString("%0 room change temp,temp:%1").arg(RoomId).arg(targetTemp);
}

void ScheduleController::sendJSON(QJsonObject ojson){
	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	qDebug()<<"向"<<curSocket->Room_id<<"发送\n"<<msg;
	sendPacket(msg);
}

void ScheduleController::sendPacket(QByteArray body){
	QByteArray head;
	// 构造头部
	int  length = body.size();
	int len_int = sizeof(length);
	head.resize(len_int);
	memcpy(head.data(), &length, len_int);
	QByteArray packet;
	packet = head + body;
	if(!curSocket->socket->isOpen())
		qDebug()<<"socket连接失效";
	curSocket->socket->write(packet, packet.size());
}

void ScheduleController::SendStopMsg(int RoomId, float fee, float totalFee, float curTemp){
	for(auto sock : allSockets){
		if(sock->Room_id == RoomId){
			curSocket = sock;
		}
	}

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, STOP_RUNNING);
	ojson.insert(CUR_TEMP, curTemp);
	ojson.insert(CUR_FEE, fee);
	ojson.insert(TOTAL_FEE, totalFee);
	sendJSON(ojson);
}

void ScheduleController::SendWorkMsg(int RoomId){
	for(auto sock : allSockets){
		if(sock->Room_id == RoomId){
			curSocket = sock;
		}
	}

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, STATE_WORK);
	sendJSON(ojson);
}

void ScheduleController::SendIdleMsg(int RoomId){
	for(auto sock : allSockets){
		if(sock->Room_id == RoomId){
			curSocket = sock;
		}
	}

	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, STATE_IDLE);
	sendJSON(ojson);
}
