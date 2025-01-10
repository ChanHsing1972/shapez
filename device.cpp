#include "device.h"

Device::Device(QWidget* parent) : QWidget(parent), posX(0), posY(0) {}

void Device::setPosition(int x, int y)
{
	posX = x;
	posY = y;
	move(x, y);
	show();
	update();
}

void Device::setRotationState(int state)
{
	rotationState = state;
	update();
}

void Device::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		emit rightClicked(this); // 发出右键点击信号
	}
}

int Device::getX()
{
	return posX;
}

int Device::getY()
{
	return posY;
}

int Device::getRotationState()
{
	return rotationState;
}

int Device::getTypeID()
{
	return typeID;
}