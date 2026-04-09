// Created by ChenXin.
// 用于启动程序，创建主窗口并显示。

#include "mainWindow.h"
#include <QApplication>
#include <QResource>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaTypeStreamOperators<QVector<QPoint>>("QVector<QPoint>");
	QResource::registerResource("./assets.qrc");
	mainWindow w;
	w.show();
	return a.exec();
}