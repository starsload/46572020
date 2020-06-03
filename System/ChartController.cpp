#include "ChartController.h"
#include "AirConditionHost.h"

ChartController::ChartController(QObject *parent):
	QObject(parent)
{
}

void ChartController::setAirConditionHost(AirConditionHost *host){
	airConditionHost = host;
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
		airConditionHost->setPara(defaultTargetTemp, maxTargetTemp, minTargetTemp,
								  highFeeRate, middleFeeRate, lowFeeRate, mode, speed);
		QJsonObject ojson;
		ojson.insert(TYPE, SET_PARA_OK);
		sendJSON(ojson);
		break;
	}
	case START_UP:
	{
		airConditionHost->startUp();
		QJsonObject ojson;
		ojson.insert(TYPE, START_UP_OK);
		sendJSON(ojson);
		break;
	}
	}

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
