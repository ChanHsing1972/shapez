#include "mainWindow.h"
#include <QApplication>
#include <QResource>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaTypeStreamOperators<QVector<QPoint>>("QVector<QPoint>");
	QResource::registerResource("./assets.qrc");
	mainWindow	w;
	w.show();
	return a.exec();
}
