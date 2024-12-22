// mineral.cpp
#include "mineral.h"

Mineral::Mineral(QVector<QVector<Device*>>* devices, QWidget* parent)
	: QWidget(parent), pixelX(0), pixelY(0), minerDirection(0), devices(devices)

{
	setFixedSize(GRID_SIZE, GRID_SIZE);
	mineralImage.load("./assets/images/cycle.png");
	moveTimer = new QTimer(this);
	checkTimer = new QTimer(this);
	connect(moveTimer, &QTimer::timeout, this, &Mineral::moving); // 每次触发定时器，移动矿物一次
	connect(checkTimer, &QTimer::timeout, this, &Mineral::checkForBelt); // 检查矿物前方是否有传送带
}

void Mineral::setPosition(int x, int y)
{
	pixelX = x;
	pixelY = y;
	move(pixelX, pixelY);
}

int Mineral::getX() const
{
	return pixelX;
}

int Mineral::getY() const
{
	return pixelY;
}

void Mineral::setDirection(int dir)
{
	minerDirection = dir;
}

int Mineral::getDirection()
{
	return minerDirection;
}

void Mineral::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	int iconX = (width() - MINERAL_SIZE) / 2;
	int iconY = (height() - MINERAL_SIZE) / 2;
	painter.drawPixmap(iconX, iconY, MINERAL_SIZE, MINERAL_SIZE, mineralImage);
}
// 触发定时器，开始移动矿物
void Mineral::startMoving()
{
	moveTimer->start(MOVE_FREQUENCY);
}

// 停止定时器，停止移动矿物
void Mineral::stopMoving()
{
	moveTimer->stop();
	// 在矿物向下或向右移动到最后时，需要修正位置，对齐到网格的边界
	if (direction == _S || direction == _D_S || direction == _A_S
		|| direction == _D || direction == _W_D || direction == _S_D)
	{
		pixelX = (pixelX / GRID_SIZE) * GRID_SIZE;
		pixelY = (pixelY / GRID_SIZE) * GRID_SIZE;
		move(pixelX, pixelY);
	}
}

// 该函数用于移动矿物一次，距离为 BELT_SPEED 个像素
void Mineral::moving()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）

	// 先根据开采器的方向，移动矿物直至下一个格子
	if (Miner* miner = dynamic_cast<Miner*>((*devices)[gridX][gridY]))
	{
		direction = minerDirection;
		qDebug() << "direction: " << direction;
		switch (direction)
		{
		case _W:
			qDebug() << "W";
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

	// 如果矿物在传送带上，根据传送带的方向移动矿物
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
		raise();
	}
	else
	{
		stopMoving();
		checkTimer->start(CHECK_FREQUENCY);
	}

	// 矿物向右或向下移动时，要随时检查矿物前方是否有传送带，
	// 如果没有，则停止移动，确保矿物最后停留在传送带上
	if (direction == _D || direction == _S_D || direction == _W_D
		|| direction == _S || direction == _A_S || direction == _D_S)
	{
		int nextGridX = gridX;
		int nextGridY = gridY;
		switch (direction)
		{
		case _D:case _S_D:case _W_D:
			nextGridX++;
			break;
		case _S:case _A_S:case _D_S:
			nextGridY++;
			break;
		}
		if (nextGridX < 0 || nextGridY < 0 || nextGridX >= devices->size() || nextGridY >= (*devices)[0].size() || !dynamic_cast<Belt*>((*devices)[nextGridX][nextGridY]))
		{
			stopMoving(); // 停止移动
			checkTimer->start(CHECK_FREQUENCY); // 开始循环检查矿物前方是否有新传送带
		}
	}

	move(pixelX, pixelY);
	show();
	update();
}

// 不断检查矿物前方是否有新的传送带，如果有，则继续移动
void Mineral::checkForBelt()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	if (Belt* belt = dynamic_cast<Belt*>((*devices)[gridX][gridY]))
	{
		if (belt->getRotationState() == direction)
		{
			checkTimer->stop();
			startMoving();
		}
	}
}