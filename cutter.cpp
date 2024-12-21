#include "cutter.h"

Cutter::Cutter(QWidget* parent) : Device(parent)
{
	switch (rotationState)
	{
	case _W: pixmap.load("./assets/images/cutter_W.png"); break;
	case _D: pixmap.load("./assets/images/cutter_D.png"); break;
	case _S: pixmap.load("./assets/images/cutter_S.png"); break;
	case _A: pixmap.load("./assets/images/cutter_A.png"); break;
	default: break;
	}
}

void Cutter::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	if (rotationState == _W || rotationState == _S)
	{
		setFixedSize(GRID_SIZE * 2, GRID_SIZE);
		painter.drawPixmap(0, 0, pixmap.scaled(GRID_SIZE * 2, GRID_SIZE, Qt::KeepAspectRatio));
	}
	else if (rotationState == _D || rotationState == _A)
	{
		setFixedSize(GRID_SIZE, GRID_SIZE * 2);
		painter.drawPixmap(0, 0, pixmap.scaled(GRID_SIZE, GRID_SIZE * 2, Qt::KeepAspectRatio));
	}
}

void Cutter::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}