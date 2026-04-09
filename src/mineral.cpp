// Created by ChenXin.
// 矿物类的实现。

#include "mineral.h"

Mineral::Mineral(QVector<QVector<Device*>>* devices, int mineralType, QWidget* parent)
	: QWidget(parent), pixelX(0), pixelY(0), minerDirection(0),
	devices(devices), mineralType(mineralType), direction(0)
{
	// 根据矿物类型设置矿物尺寸
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

	// 使用映射加载矿物图片
	QMap<int, QString> mineralImages = {
			{CYCLE_MINE,    "./assets/images/cycle.png"},
			{RECT_MINE,     "./assets/images/rect.png"},
			{CYCLE_MINE_L,  "./assets/images/left_cycle.png"},
			{CYCLE_MINE_R,  "./assets/images/right_cycle.png"},
			{RECT_MINE_L,   "./assets/images/left_rect.png"}, // 虽然题目要求不能切割方形矿物，但仍然添加了切割方形矿物的接口，仅供娱乐
			{RECT_MINE_R,   "./assets/images/right_rect.png"}
	};
	mineralImage.load(mineralImages.value(mineralType));

	// 创建定时器
	moveTimer = new QTimer(this);
	checkTimer = new QTimer(this);
	connect(moveTimer, &QTimer::timeout, this, &Mineral::moving); // 每次触发定时器时，移动矿物一次
	connect(checkTimer, &QTimer::timeout, this, &Mineral::checkForBelt); // 检查矿物前方是否有传送带
}

void Mineral::setPosition(int x, int y)
{
	pixelX = x;
	pixelY = y;
	move(pixelX, pixelY);
}

int Mineral::getX() const { return pixelX; }
int Mineral::getY() const { return pixelY; }
void Mineral::setDirection(int dir) { minerDirection = dir; }
int Mineral::getDirection() const { return minerDirection; }

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
	// 矿物停止移动后，确保位置在格子中心
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

	// 1. 首先检查矿物是否在地图范围内
	if (pixelX <= 0 || pixelY <= 0 || pixelX >= WINDOW_WIDTH || pixelY >= WINDOW_HEIGHT)
	{
		qDebug() << "STOP 0: Out of bonnd!";
		return;
	}

	// 2. 根据开采器的方向移动矿物
	if (Miner* miner = dynamic_cast<Miner*>((*devices)[gridX][gridY]))
	{
		direction = minerDirection;
	}
	// a. 如果矿物在传送带上，则根据传送带的方向移动矿物
	else if (Belt* belt = dynamic_cast<Belt*>((*devices)[gridX][gridY]))
	{
		if (offsetX == 0 && offsetY == 0) // 确保矿物走完一个格子再转弯
		{
			int newDirection = belt->getRotationState();
			if (direction != newDirection) // 如果矿物的方向和传送带的方向不一致，则更新矿物的方向
			{
				direction = newDirection;
				directionChanged = true; // 记录方向已改变
			}
		}
	}
	else if (dynamic_cast<Hub*>((*devices)[gridX][gridY])
		|| dynamic_cast<Trash*>((*devices)[gridX][gridY])) {
	}
	// b. 如果矿物不在设备上，则直接删除
	else
	{
		qDebug() << "STOP 1: Not on devices!";
		hide(); // 隐藏矿物
		auto it = std::find_if(GameMap::mineralList.begin(), GameMap::mineralList.end(),
			[this](Mineral* m) { return m == this; }); // 从 mineralList 中移除矿物对象
		if (it != GameMap::mineralList.end())
		{
			GameMap::mineralList.erase(it); // 从容器中移除对象
		}
		this->deleteLater(); // 删除矿物
		return;
	}

	// 3. 如果方向改变，则本次只更新方向，等待下一次移动
	if (directionChanged)
	{
		move(pixelX, pixelY);
		show();
		update();
		return;
	}

	// 4. 检查矿物是否到达垃圾桶、切割机或交付中心
	if (checkIfAtTrash() || checkIfAtCutter() || checkIfAtHub())
	{
		hide(); // 隐藏矿物
		auto it = std::find_if(GameMap::mineralList.begin(), GameMap::mineralList.end(),
			[this](Mineral* m) { return m == this; }); // 从 mineralList 中移除矿物对象
		if (it != GameMap::mineralList.end())
		{
			GameMap::mineralList.erase(it); // 从容器中移除对象
		}
		this->deleteLater(); // 删除矿物
		return;
	}

	// 5. 检查矿物前方是否有矿物，实现矿物之间的碰撞检测
	// 6. 检查矿物前方是否有传送带，如果没有，则停止移动，确保矿物最后停留在传送带上
	if (isMineralAhead() || !isBeltAhead())
	{
		stopMoving(); // 停止移动
		checkTimer->start(CHECK_FREQUENCY); // 开始循环检查矿物前方是否有新传送带
		return;
	}

	// 7. 完成上述所有检查操作后，移动矿物
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
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int nextGridX = getNextGridX(pixelX / GRID_SIZE);
	int nextGridY = getNextGridY(pixelY / GRID_SIZE);

	// 检查地图边界
	if (nextGridX < 0 || nextGridY < 0 || nextGridX >= WINDOW_WIDTH / GRID_SIZE || nextGridY >= WINDOW_HEIGHT / GRID_SIZE)
	{
		qDebug() << "STOP 2: Mineral ahead!";
		return true; // 前方无路，视为有障碍
	}

	// 检查前方是否有矿物
	for (const auto& mineral : GameMap::mineralList)
	{
		if (mineral != this && mineral->getX() / GRID_SIZE == nextGridX
			&& mineral->getY() / GRID_SIZE == nextGridY
			&& offsetX == 0 && offsetY == 0 && mineral->getX() % GRID_SIZE == 0 && mineral->getY() % GRID_SIZE == 0)
		{
			qDebug() << "STOP 2: Mineral ahead!";
			return true;
		}
	}
	return false;
}

// 检查前方是否有传送带
bool Mineral::isBeltAhead()
{
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int nextGridX = getNextGridX(pixelX / GRID_SIZE);
	int nextGridY = getNextGridY(pixelY / GRID_SIZE);

	// 检查地图边界
	if (nextGridX < 0 || nextGridY < 0 || nextGridX >= WINDOW_WIDTH / GRID_SIZE || nextGridY >= WINDOW_HEIGHT / GRID_SIZE)
	{
		qDebug() << "STOP 3: No belt ahead!";
		return false; // 前方无路，视为无传送带
	}

	if ((offsetX == 0 && offsetY == 0))
	{
		if (!dynamic_cast<Belt*>((*devices)[nextGridX][nextGridY])
			&& !dynamic_cast<Hub*>((*devices)[nextGridX][nextGridY])
			&& !dynamic_cast<Trash*>((*devices)[nextGridX][nextGridY]))
		{
			qDebug() << "STOP 3: No belt ahead!";
			return false;
		}
	}
	return true;
}

// 检查矿物是否到达交付中心
bool Mineral::checkIfAtHub()
{
	int gridX = pixelX / GRID_SIZE;
	int gridY = pixelY / GRID_SIZE;
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）

	// 交付中心的位置为
	// (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + i, (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + j
	if (offsetX == 0 && offsetY == 0
		&& ((gridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 && gridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2)
			|| (gridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + 1 && gridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2)
			|| (gridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 && gridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + 1)
			|| (gridX == (WINDOW_WIDTH / GRID_SIZE - 2) / 2 + 1 && gridY == (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + 1)))
	{
		qDebug() << "Mineral delivered!";
		if (Hub* hub = dynamic_cast<Hub*>((*devices)[gridX][gridY]))
		{
			hub->increaseMineralCount(mineralType);
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
	int offsetX = pixelX % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	if (Trash* trash = dynamic_cast<Trash*>((*devices)[gridX][gridY]))
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
	int offsetY = pixelY % GRID_SIZE; // 矿物在格子里的偏移量（单位：像素）
	if (gridX < 0 || gridX >= devices->size() || gridY < 0 || gridY >= devices->at(0).size())
	{
		return false; // 矿物不在地图范围内
	}

	// 如果到达切割机
	if (Cutter* cutter = dynamic_cast<Cutter*>((*devices)[nextGridX][nextGridY]))
	{
		if (offsetX == 0 && offsetY == 0 && direction == cutter->getRotationState())
		{
			if (mineralType == RECT_MINE)
			{
				return false;
			}
			// directions 存储方向对应的偏移量
			const std::map<int, std::pair<int, int>> directions = {
				{_W, {1, 0}},
				{_A, {0, -1}},
				{_S, {-1, 0}},
				{_D, {0, 1}}
			};
			int rotationState = cutter->getRotationState();
			int outputOffsetX = directions.at(rotationState).first;
			int outputOffsetY = directions.at(rotationState).second;

			// 先判断切割机堵塞情况
			// 如果切割机前方有矿物，则堵塞
			if (isMineralAt(outputGridX, outputGridY)
				|| isMineralAt(outputGridX + outputOffsetX, outputGridY + outputOffsetY))
			{
				return false;
			}

			// 检查切割机 1 口和 2 口前方的设备
			Device* device1 = (*devices)[outputGridX][outputGridY];
			Device* device2 = (*devices)[outputGridX + outputOffsetX][outputGridY + outputOffsetY];

			// 前方只有在两传送带、一传送带一垃圾桶、两垃圾桶时为 true，其他时候均为 false
			bool condition1 = dynamic_cast<Belt*>(device1) && dynamic_cast<Belt*>(device2);
			bool condition2 = (dynamic_cast<Belt*>(device1) && dynamic_cast<Trash*>(device2))
				|| (dynamic_cast<Trash*>(device1) && dynamic_cast<Belt*>(device2));
			bool condition3 = dynamic_cast<Trash*>(device1) && dynamic_cast<Trash*>(device2);
			if (!condition1 && !condition2 && !condition3)
			{
				return false;
			}

			// 切割机未堵塞，可以切割矿物
			// 切割机切割矿物是一个耗时操作，需要等待数秒
			QEventLoop loop;
			QTimer::singleShot(CUTTER_SPEED, &loop, &QEventLoop::quit);
			loop.exec();

			Mineral* halfMineral1 = nullptr;
			Mineral* halfMineral2 = nullptr;
			if (mineralType == CYCLE_MINE)
			{
				halfMineral1 = new Mineral(devices, CYCLE_MINE_L, parentWidget());
				halfMineral2 = new Mineral(devices, CYCLE_MINE_R, parentWidget());
			}
			else if (mineralType == RECT_MINE)
			{
				halfMineral1 = new Mineral(devices, RECT_MINE_L, parentWidget());
				halfMineral2 = new Mineral(devices, RECT_MINE_R, parentWidget());
			}

			// 设置切割后新矿物的位置和方向
			halfMineral1->setPosition(outputGridX * GRID_SIZE, outputGridY * GRID_SIZE);
			halfMineral2->setPosition((outputGridX + outputOffsetX) * GRID_SIZE, (outputGridY + outputOffsetY) * GRID_SIZE);
			halfMineral1->setDirection(rotationState);
			halfMineral2->setDirection(rotationState);
			GameMap::mineralList.append(halfMineral1); // 将新生成的矿物添加到全局列表
			GameMap::mineralList.append(halfMineral2); // 将新生成的矿物添加到全局列表
			halfMineral1->startMoving();
			halfMineral2->startMoving();
			return true;
		}
	}
	return false;
}

// 检查指定位置是否有矿物
bool Mineral::isMineralAt(int gridX, int gridY)
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
	if (nextGridX < 0 || nextGridX >= WINDOW_WIDTH / GRID_SIZE)
	{
		return 0;
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
	if (nextGridY < 0 || nextGridY >= WINDOW_HEIGHT / GRID_SIZE)
	{
		return 0;
	}
	return nextGridY;
}