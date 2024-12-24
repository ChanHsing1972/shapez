// mineral.cpp
#include "mineral.h"

Mineral::Mineral(QVector<QVector<Device*>>* devices, int mineralType, QWidget* parent)
	: QWidget(parent), pixelX(0), pixelY(0), minerDirection(0), devices(devices)
{
	switch (mineralType)
	{
	case CYCLE_MINE:case RECT_MINE:
		setFixedSize(GRID_SIZE, GRID_SIZE);
		break;
	case CYCLE_MINE_L:case RECT_MINE_L:case CYCLE_MINE_R:case RECT_MINE_R:
		setFixedSize(GRID_SIZE / 2, GRID_SIZE);
		break;
	default:
		break;
	}

	switch (mineralType)
	{
	case CYCLE_MINE:
		mineralImage.load("./assets/images/cycle.png");
		break;
	case RECT_MINE:
		mineralImage.load("./assets/images/rect.png");
		break;
	case CYCLE_MINE_L:
		mineralImage.load("./assets/images/left_cycle.png");
		break;
	case CYCLE_MINE_R:
		mineralImage.load("./assets/images/right_cycle.png");
		break;
	case RECT_MINE_L:
		mineralImage.load("./assets/images/left_rect.png");
		break;
	case RECT_MINE_R:
		mineralImage.load("./assets/images/right_rect.png");
		break;
	default:
		break;
	}

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
	int iconX = (width() - mineralImage.width() / 2) / 2;
	int iconY = (height() - mineralImage.height() / 2) / 2;
	painter.drawPixmap(iconX, iconY, mineralImage.width() / 2, mineralImage.height() / 2, mineralImage);
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
	// 矿物停止移动后，确保位置在格子中心（以防万一）
	pixelX = (pixelX / GRID_SIZE) * GRID_SIZE;
	pixelY = (pixelY / GRID_SIZE) * GRID_SIZE;
	move(pixelX, pixelY);
}

// 该函数用于移动矿物一次，每次移动距离为 BELT_SPEED 个像素
void Mineral::moving()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	bool directionChanged = false; // 标记方向是否改变

	// 首先检查矿物是否在地图范围内
	if (pixelX < 0 || pixelY < 0 || pixelX >= WINDOW_WIDTH || pixelY >= WINDOW_HEIGHT)
	{
		qDebug() << "STOP 0";
		return;
	}

	// 先根据开采器的方向移动矿物
	if (Miner* miner = dynamic_cast<Miner*>((*devices)[gridX][gridY]))
	{
		direction = minerDirection;
	}
	// 如果矿物在传送带上，则根据传送带的方向移动矿物
	else if (Belt* belt = dynamic_cast<Belt*>((*devices)[gridX][gridY]))
	{
		if (offsetX == 0 && offsetY == 0) // 确保矿物走完一个格子再转弯
		{
			int newDirection = belt->getRotationState();
			if (direction != newDirection)
			{
				direction = newDirection;
				directionChanged = true; // 记录方向已改变
			}
		}
	}
	// 如果矿物不在这两个设备上，则停止移动
	else
	{
		qDebug() << "STOP 1";
		stopMoving(); // 停止移动
		checkTimer->start(CHECK_FREQUENCY); // 开始循环检查矿物前方是否有新传送带
		return;
	}

	// 如果方向改变，则本次只更新方向，等待下一次移动
	if (directionChanged)
	{
		move(pixelX, pixelY);
		show();
		update();
		return;
	}

	// 检查矿物前方是否有矿物，实现矿物之间的碰撞检测
	if (isMineralAhead())
	{
		qDebug() << "STOP 2";
		stopMoving(); // 停止移动
		checkTimer->start(CHECK_FREQUENCY); // 开始循环检查矿物前方是否有新传送带
		return;
	}

	// 检查矿物是否到达垃圾桶、切割机或交付中心
	if (checkIfAtTrash() || checkIfAtCutter() || checkIfAtHub())
	{
		hide(); // 隐藏矿物
		// 从 mineralList 中移除矿物对象
		auto it = std::find_if(GameMap::mineralList.begin(), GameMap::mineralList.end(),
			[this](Mineral* m) { return m == this; });
		if (it != GameMap::mineralList.end())
		{
			GameMap::mineralList.erase(it); // 从容器中移除对象
		}
		this->deleteLater(); // 删除矿物
		return;
	}

	// 检查矿物前方是否有传送带，如果没有，则停止移动，确保矿物最后停留在传送带上
	int nextGridX = getNextGridX(gridX);
	int nextGridY = getNextGridY(gridY);
	if ((nextGridX < 0 || nextGridY < 0 || nextGridX >= devices->size() || nextGridY >= (*devices)[0].size()
		|| !dynamic_cast<Belt*>((*devices)[nextGridX][nextGridY])) && (offsetX == 0 && offsetY == 0))
	{
		qDebug() << "STOP 3";
		stopMoving(); // 停止移动
		checkTimer->start(CHECK_FREQUENCY); // 开始循环检查矿物前方是否有新传送带
		return;
	}

	// 完成上述所有检查操作后，移动矿物
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
	raise(); // 确保矿物始终显示在传送带上方
	move(pixelX, pixelY);
	show();
	update();
}

// 循环检查矿物前方是否有新的传送带，如果有，则继续移动
void Mineral::checkForBelt()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	if (Belt* belt = dynamic_cast<Belt*>((*devices)[gridX][gridY]))
	{
		if (belt->getRotationState() == direction && !isMineralAhead())
		{
			checkTimer->stop();
			startMoving();
			return;
		}
	}
}

// 检查前方是否有矿物
bool Mineral::isMineralAhead()
{
	int nextGridX = pixelX / GRID_SIZE;
	int nextGridY = pixelY / GRID_SIZE;
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）

	switch (direction)
	{
	case _W:case _D_W:case _A_W:
		nextGridY--;
		break;
	case _D:case _S_D:case _W_D:
		nextGridX++;
		break;
	case _S:case _A_S:case _D_S:
		nextGridY++;
		break;
	case _A:case _W_A:case _S_A:
		nextGridX--;
		break;
	}
	// 检查地图边界
	if (nextGridX < 0 || nextGridY < 0 || nextGridX >= WINDOW_WIDTH / GRID_SIZE || nextGridY >= WINDOW_HEIGHT / GRID_SIZE)
	{
		return true; // 前方无路，视为有障碍
	}

	for (const auto& mineral : GameMap::mineralList)
	{
		if (mineral != this && (mineral->getX() / GRID_SIZE == nextGridX)
			&& (mineral->getY() / GRID_SIZE == nextGridY)
			&& offsetX == 0 && offsetY == 0)
		{
			return true;
		}
	}
	return false;
}

// 检查矿物是否到达交付中心
bool Mineral::checkIfAtHub()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int nextGridX = getNextGridX(gridX);
	int nextGridY = getNextGridY(gridY);
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）

	// 交付中心的位置为
	// (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + i, (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + j
	if (offsetX == 0 && offsetY == 0
		&& ((nextGridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 && nextGridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2)
			|| (nextGridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + 1 && nextGridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2)
			|| (nextGridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 && nextGridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + 1)
			|| (nextGridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + 1 && nextGridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + 1)))
	{
		qDebug() << "Mineral delivered!";
		if (Hub* hub = dynamic_cast<Hub*>((*devices)[nextGridX][nextGridY]))
		{
			hub->increaseCount();
		}
		return true;
	}
	return false;
}

// 检查矿物是否到达垃圾桶
bool Mineral::checkIfAtTrash()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int nextGridX = getNextGridX(gridX);
	int nextGridY = getNextGridY(gridY);
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	if (Trash* trash = dynamic_cast<Trash*>((*devices)[nextGridX][nextGridY]))
	{
		if (offsetX == 0 && offsetY == 0)
		{
			return true;
		}
	}
	return false;
}

// 检查矿物是否到达切割机
bool Mineral::checkIfAtCutter()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int nextGridX = getNextGridX(gridX);
	int nextGridY = getNextGridY(gridY);
	int outputGridX = getNextGridX(nextGridX);
	int outputGridY = getNextGridY(nextGridY);
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素
	if (gridX < 0 || gridX >= devices->size() || gridY < 0 || gridY >= devices->at(0).size())
	{
		return false;
	}
	if (Cutter* cutter = dynamic_cast<Cutter*>((*devices)[nextGridX][nextGridY]))
	{
		if (offsetX == 0 && offsetY == 0 && direction == cutter->getRotationState())
		{
			if (isMineral(outputGridX, outputGridY) || isMineral(outputGridX, outputGridY + 1))
			{
				return false;
			}
			if (Belt* belt = dynamic_cast<Belt*>((*devices)[outputGridX][outputGridY])) {}
			else
			{
				return false;
			}
			if (Belt* belt = dynamic_cast<Belt*>((*devices)[outputGridX][outputGridY + 1])) {}
			else
			{
				return false;
			}
			qDebug() << "Mineral cut!";
			Mineral* halfMineral1 = new Mineral(devices, CYCLE_MINE_L, parentWidget());
			Mineral* halfMineral2 = new Mineral(devices, CYCLE_MINE_R, parentWidget());
			halfMineral1->setPosition(nextGridX * GRID_SIZE + 60, nextGridY * GRID_SIZE);
			halfMineral2->setPosition(nextGridX * GRID_SIZE + 60, nextGridY * GRID_SIZE + 60);
			halfMineral1->setDirection(cutter->getRotationState());
			halfMineral2->setDirection(cutter->getRotationState());
			GameMap::mineralList.append(halfMineral1); // 将新生成的矿物添加到全局列表
			GameMap::mineralList.append(halfMineral2); // 将新生成的矿物添加到全局列表
			halfMineral1->show();
			halfMineral2->show();
			halfMineral1->startMoving();
			halfMineral2->startMoving();
			qDebug() << "Mineral generated!";
			return true;
		}
	}
	return false;
}

bool Mineral::isMineral(int gridX, int gridY)
{
	for (const auto& mineral : GameMap::mineralList)
	{
		if (mineral != this && (mineral->getX() / GRID_SIZE == gridX) && (mineral->getY() / GRID_SIZE == gridY))
		{
			return true;
		}
	}
	return false;
}


int Mineral::getNextGridX(int gridX)
{
	int nextGridX = gridX;
	switch (direction)
	{
	case _A:case _W_A:case _S_A:
		nextGridX--;
		break;
	case _D:case _S_D:case _W_D:
		nextGridX++;
		break;
	}
	return nextGridX;
}

int Mineral::getNextGridY(int gridY)
{
	int nextGridY = gridY;
	switch (direction)
	{
	case _W:case _D_W:case _A_W:
		nextGridY--;
		break;
	case _S:case _A_S:case _D_S:
		nextGridY++;
		break;
	}
	return nextGridY;
}