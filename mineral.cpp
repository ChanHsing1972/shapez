// mineral.cpp
#include "mineral.h"

Mineral::Mineral(QVector<QVector<Device*>>* devices, QWidget* parent)
	: QWidget(parent), pixelX(0), pixelY(0), minerDirection(0), devices(devices)

{
	setFixedSize(GRID_SIZE, GRID_SIZE);
	mineralImage.load("./assets/images/cycle.png");
	moveTimer = new QTimer(this);
	connect(moveTimer, &QTimer::timeout, this, &Mineral::moving);
}

void Mineral::setPosition(int x, int y)
{
	pixelX = x;
	pixelY = y;
	move(pixelX, pixelY);
}

void Mineral::setDirection(int dir)
{
	minerDirection = dir;
}

int Mineral::getDirection()
{
	return minerDirection;
}

void Mineral::startMoving()
{
	moveTimer->start(MOVE_FREQUENCY);
}

void Mineral::stopMoving()
{
	moveTimer->stop();
}

int Mineral::getX() const
{
	return pixelX;
}

int Mineral::getY() const
{
	return pixelY;
}

void Mineral::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	int iconX = (width() - MINERAL_SIZE) / 2;
	int iconY = (height() - MINERAL_SIZE) / 2;
	painter.drawPixmap(iconX, iconY, MINERAL_SIZE, MINERAL_SIZE, mineralImage);
}

void Mineral::moving()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	qDebug() << "Mineral at" << gridX << gridY;
	qDebug() << "Direction is" << direction;
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	if (Miner* miner = dynamic_cast<Miner*>((*devices)[gridX][gridY]))
	{
		direction = minerDirection;
		switch (direction)
		{
		case _W:
			pixelY -= BELT_SPEED;
			break;
		case _D:
			pixelX += BELT_SPEED;
			break;
		case _S:
			pixelY += BELT_SPEED;
			break;
		case _A:
			pixelX -= BELT_SPEED;
			break;
		}
	}
	else if (Belt* belt = dynamic_cast<Belt*>((*devices)[gridX][gridY]))
	{
		if (offsetX == 0 && offsetY == 0) // 确保矿物走完一个格子再转弯
			direction = belt->getRotationState();
		switch (direction)
		{
		case _W:case _D_W:case _A_W:
			pixelY -= BELT_SPEED;
			break;
		case _D:case _S_D:case _W_D:
			pixelX += BELT_SPEED;
			break;
		case _S:case _A_S:case _D_S:
			pixelY += BELT_SPEED;
			break;
		case _A:case _W_A:case _S_A:
			pixelX -= BELT_SPEED;
			break;
		}
	}
	else
	{
		moveTimer->stop();
	}
	move(pixelX, pixelY);
	show();
	update();
}

void Mineral::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		isDeleting = true;
		hide();
		update();
	}
}

void Mineral::mouseMoveEvent(QMouseEvent* event)
{
	if (isDeleting)
	{
		hide();
		update();
	}
}

void Mineral::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		isDeleting = false;
	}
}