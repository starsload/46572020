#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
//	MainWindow b;
//	b.show();
//	MainWindow c;
//	c.show();
//	MainWindow d;
//	d.show();
	return a.exec();
}
