#include "cutter.h"

Cutter::Cutter(QVector<QVector<Device*>>* devices, QWidget* parent)
	: Device(parent), devices(devices), pixmap(4)
{
	pixmap[_W].load("./assets/images/cutter_W.png");
	pixmap[_D].load("./assets/images/cutter_D.png");
	pixmap[_S].load("./assets/images/cutter_S.png");
	pixmap[_A].load("./assets/images/cutter_A.png");
}

void Cutter::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	lower(); // 使切割机位于最底层

	if (rotationState == _W || rotationState == _S)
	{
		setFixedSize(GRID_SIZE * 2, GRID_SIZE);
		painter.drawPixmap(0, 0, pixmap[rotationState].scaled(GRID_SIZE * 2, GRID_SIZE, Qt::KeepAspectRatio));
	}
	else if (rotationState == _D || rotationState == _A)
	{
		setFixedSize(GRID_SIZE, GRID_SIZE * 2);
		painter.drawPixmap(0, 0, pixmap[rotationState].scaled(GRID_SIZE, GRID_SIZE * 2, Qt::KeepAspectRatio));
	}
}

void Cutter::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}