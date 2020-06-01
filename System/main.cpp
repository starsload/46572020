#include <QCoreApplication>
#include <myserver.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	MyServer *server = new MyServer();
	if(!server->listen(QHostAddress::Any, 6666))
	{
		qDebug()<<"服务器监听失败";
		return 0;
	}
	else
	{
		qDebug()<<"开始监听";
	}
	return a.exec();
}
