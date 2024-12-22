#include "gameMap.h"

QVector<Mineral*> GameMap::mineralList; // 初始化全局矿物对象列表

GameMap::GameMap(QWidget* parent) : QWidget(parent),
beltToPlace(12), minerToPlace(4), cutterToPlace(4), trashToPlace(4)
{
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setMouseTracking(true); // 启用鼠标跟踪

	// 加载资源文件
	cycleMineImage.load("./assets/images/cycle_mine.png");
	rectMineImage.load("./assets/images/rect_mine.png");
	barrierImage.load("./assets/images/barrier.png");
	hubSmallImage.load("./assets/images/hub_small.png");

	beltToPlace[_W].load("./assets/images/belt_blue_W.png");
	beltToPlace[_D].load("./assets/images/belt_blue_D.png");
	beltToPlace[_S].load("./assets/images/belt_blue_S.png");
	beltToPlace[_A].load("./assets/images/belt_blue_A.png");

	beltToPlace[_W_D].load("./assets/images/belt_blue_W_D.png");
	beltToPlace[_D_S].load("./assets/images/belt_blue_D_S.png");
	beltToPlace[_S_A].load("./assets/images/belt_blue_S_A.png");
	beltToPlace[_A_W].load("./assets/images/belt_blue_A_W.png");

	beltToPlace[_W_A].load("./assets/images/belt_blue_W_A.png");
	beltToPlace[_A_S].load("./assets/images/belt_blue_A_S.png");
	beltToPlace[_S_D].load("./assets/images/belt_blue_S_D.png");
	beltToPlace[_D_W].load("./assets/images/belt_blue_D_W.png");

	cutterToPlace[_W].load("./assets/images/cutter_blue_W.png");
	cutterToPlace[_D].load("./assets/images/cutter_blue_D.png");
	cutterToPlace[_S].load("./assets/images/cutter_blue_S.png");
	cutterToPlace[_A].load("./assets/images/cutter_blue_A.png");

	minerToPlace[_W].load("./assets/images/miner_blue_W.png");
	minerToPlace[_D].load("./assets/images/miner_blue_D.png");
	minerToPlace[_S].load("./assets/images/miner_blue_S.png");
	minerToPlace[_A].load("./assets/images/miner_blue_A.png");

	trashToPlace[_W].load("./assets/images/trash_blue_W.png");
	trashToPlace[_D].load("./assets/images/trash_blue_D.png");
	trashToPlace[_S].load("./assets/images/trash_blue_S.png");
	trashToPlace[_A].load("./assets/images/trash_blue_A.png");

	// 初始化地图，并加入地图列表
	MapData mapA;
	initializeMapA(mapA);
	maps.append(mapA);

	// 加载第一个地图
	loadMap(0);

	// 创建按钮
	QPushButton* beltButton = new QPushButton(this);
	QPushButton* minerButton = new QPushButton(this);
	QPushButton* cutterButton = new QPushButton(this);
	QPushButton* trashButton = new QPushButton(this);
	QPushButton* shopButton = new QPushButton(this);
	QPushButton* helpButton = new QPushButton(this);

	// 加载按钮图片
	beltButton->setIcon(QIcon("./assets/images/belt_button.png"));
	minerButton->setIcon(QIcon("./assets/images/miner_button.png"));
	cutterButton->setIcon(QIcon("./assets/images/cutter_button.png"));
	trashButton->setIcon(QIcon("./assets/images/trash_button.png"));
	shopButton->setIcon(QIcon("./assets/images/shop_button.png"));
	helpButton->setIcon(QIcon("./assets/images/help_button.png"));

	// 设置图标大小
	beltButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
	minerButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
	cutterButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
	trashButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
	shopButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
	helpButton->setIconSize(QSize(ICON_SIZE, ICON_SIZE));

	// 设置按钮位置及大小
	beltButton->setGeometry(BELT_BUTTON_X, BELT_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);
	minerButton->setGeometry(MINER_BUTTON_X, MINER_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);
	cutterButton->setGeometry(CUTTER_BUTTON_X, CUTTER_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);
	trashButton->setGeometry(TRASH_BUTTON_X, TRASH_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);
	shopButton->setGeometry(SHOP_BUTTON_X, SHOP_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);
	helpButton->setGeometry(HELP_BUTTON_X, HELP_BUTTON_Y, BUTTON_SIZE, BUTTON_SIZE);

	// 连接按钮的信号和槽
	connect(beltButton, &QPushButton::clicked, this, [this]() { onButtonClicked("Belt"); });
	connect(minerButton, &QPushButton::clicked, this, [this]() { onButtonClicked("Miner"); });
	connect(cutterButton, &QPushButton::clicked, this, [this]() { onButtonClicked("Cutter"); });
	connect(trashButton, &QPushButton::clicked, this, [this]() { onButtonClicked("Trash"); });
	connect(helpButton, &QPushButton::clicked, this, [this]() {
		HelpPage* helpPage = new HelpPage(this);
		helpPage->exec();
		});
}

GameMap::~GameMap() {}

// 初始化地图
void GameMap::initializeMapA(MapData& mapA)
{
	for (int i = 3; i <= 8; i++)
		mapA.cycleMines.append(QPoint(i, 2));
	for (int i = 2; i <= 8; i++)
		mapA.cycleMines.append(QPoint(i, 3));
	for (int i = 2; i <= 8; i++)
		mapA.cycleMines.append(QPoint(i, 4));
	for (int i = 5; i <= 7; i++)
		mapA.cycleMines.append(QPoint(i, 5));
	for (int i = 5; i <= 8; i++)
		mapA.rectMines.append(QPoint(25, i));
	for (int i = 6; i <= 8; i++)
		mapA.rectMines.append(QPoint(26, i));
	for (int i = 7; i <= 9; i++)
		mapA.rectMines.append(QPoint(27, i));
	mapA.barriers.append(QPoint(27, 11));
	mapA.barriers.append(QPoint(30, 11));
	mapA.barriers.append(QPoint(10, 11));
	mapA.barriers.append(QPoint(2, 5));
	mapA.barriers.append(QPoint(9, 9));
	mapA.barriers.append(QPoint(20, 7));
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			mapA.hubSmall.append(QPoint((WINDOW_WIDTH / GRID_SIZE - 2) / 2 + i, (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + j));
		}
	}
}

void GameMap::initializeMapB(MapData& mapB) {}

void GameMap::initializeMapC(MapData& mapB) {}

// 加载地图
void GameMap::loadMap(int level)
{
	currentMap = maps[level];
	update();
}

// 绘制地图
void GameMap::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QColor gridColor(GRAY);

	// 生成网格
	for (int x = 0; x < width(); x += GRID_SIZE)
	{
		painter.setPen(gridColor);
		painter.drawLine(x, 0, x, height());
	}

	for (int y = 0; y < height(); y += GRID_SIZE)
	{
		painter.setPen(gridColor);
		painter.drawLine(0, y, width(), y);
	}

	// 绘制矿物
	for (const QPoint& mine : currentMap.cycleMines)
	{
		int x = mine.x() * GRID_SIZE;
		int y = mine.y() * GRID_SIZE;
		painter.drawPixmap(x, y, cycleMineImage.scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}

	for (const QPoint& mine : currentMap.rectMines)
	{
		int x = mine.x() * GRID_SIZE;
		int y = mine.y() * GRID_SIZE;
		painter.drawPixmap(x, y, rectMineImage.scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}

	// 绘制障碍
	for (const QPoint& barrier : currentMap.barriers)
	{
		int x = barrier.x() * GRID_SIZE;
		int y = barrier.y() * GRID_SIZE;
		painter.drawPixmap(x, y, barrierImage.scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}

	// 绘制小型中心（小型中心用数组存储它占用的格子）
	int hubSmallX = currentMap.hubSmall[0].x() * GRID_SIZE;
	int hubSmallY = currentMap.hubSmall[0].y() * GRID_SIZE;
	painter.drawPixmap(hubSmallX, hubSmallY, hubSmallImage.scaled(GRID_SIZE * 2, GRID_SIZE * 2, Qt::KeepAspectRatio));

	// 绘制“待放置”反馈图标
	if (canPlaceBelt)
	{
		painter.drawPixmap(mousePosition.x() - GRID_SIZE / 2, mousePosition.y() - GRID_SIZE / 2,
			beltToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
	if (canPlaceMiner)
	{
		painter.drawPixmap(mousePosition.x() - GRID_SIZE / 2, mousePosition.y() - GRID_SIZE / 2,
			minerToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
	if (canPlaceCutter)
	{
		if (rotationState == _W || rotationState == _S)
		{
			painter.drawPixmap(mousePosition.x() - GRID_SIZE / 2, mousePosition.y() - GRID_SIZE / 2,
				cutterToPlace[rotationState].scaled(GRID_SIZE * 2, GRID_SIZE, Qt::KeepAspectRatio));
		}
		else if (rotationState == _D || rotationState == _A)
		{
			painter.drawPixmap(mousePosition.x() - GRID_SIZE / 2, mousePosition.y() - GRID_SIZE / 2,
				cutterToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE * 2, Qt::KeepAspectRatio));
		}
	}
	if (canPlaceTrash)
	{
		painter.drawPixmap(mousePosition.x() - GRID_SIZE / 2, mousePosition.y() - GRID_SIZE / 2,
			trashToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
}

// 鼠标单击某按钮后，标记为可以放下该物品
void GameMap::onButtonClicked(QString s)
{
	qDebug() << s << "button clicked!";
	canPlaceBelt = (s == "Belt" ? true : false);
	canPlaceMiner = (s == "Miner" ? true : false);
	canPlaceCutter = (s == "Cutter" ? true : false);
	canPlaceTrash = (s == "Trash" ? true : false);
}

// 鼠标移动事件：更新光标 & 传送带拖动放置 & 拖动删除物品
void GameMap::mouseMoveEvent(QMouseEvent* event)
{
	// 只在鼠标位置改变时更新，减少重绘次数
	QPoint newMousePosition = event->pos();
	if (newMousePosition != mousePosition)
	{
		mousePosition = newMousePosition;
		if (canPlaceBelt || canPlaceMiner || canPlaceCutter || canPlaceTrash)
		{
			update();
		}
	}

	// 传送带拖动放置
	if (isPlacingBelt)
	{
		// 如果鼠标进入一个新的格子，则 isDirectionChanged 重置为 false，默认格子为空
		if (!beltList.empty())
		{
			int currentMouseGridX = mousePosition.x() / GRID_SIZE;
			int currentMouseGridY = mousePosition.y() / GRID_SIZE;
			int lastBeltGridX = beltList.back()->getX() / GRID_SIZE;
			int lastBeltGridY = beltList.back()->getY() / GRID_SIZE;
			if (currentMouseGridX != lastBeltGridX || currentMouseGridY != lastBeltGridY)
			{
				isDirectionChanged = false;
				isEmptyGrid = true;
				placeBeltAt(event->pos()); // 先按照原来的方向，放置一个传送带，该过程将同时判断格子是否为空
			}
		}
		if (isDirectionChanged == false && isEmptyGrid == true)
		{
			getBeltDirection(event->pos()); // 检查此传送带方向是否需要更改
		}
	}

	// 拖动删除物品
	if (isDeleting)
	{
		deleteDeviceAt(event->pos());
		for (auto it = mineralList.begin(); it != mineralList.end();)
		{
			Mineral* mineral = *it;
			if (mineral->geometry().contains(newMousePosition))
			{
				mineral->hide();
				mineral->update();
				mineral->deleteLater();
				it = mineralList.erase(it); // 从容器中移除对象
			}
			else
			{
				++it;
			}
		}
		update();
	}
}

// 左 / 右键单击事件：放置物品 / 删除物品及取消选中
void GameMap::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (canPlaceBelt)
		{
			isPlacingBelt = true;
			placeBeltAt(event->pos());
		}
		if (canPlaceMiner)
		{
			placeMinerAt(event->pos());
		}
		if (canPlaceCutter)
		{
			placeCutterAt(event->pos());
		}
		if (canPlaceTrash)
		{
			placeTrashAt(event->pos());
		}
	}

	if (event->button() == Qt::RightButton)
	{
		isDeleting = true;
		canPlaceBelt = false;
		canPlaceMiner = false;
		canPlaceCutter = false;
		canPlaceTrash = false;
		deleteDeviceAt(event->pos());
		update();
	}
}

// 左 / 右键抬起事件：结束放置传送带 / 结束删除物品
void GameMap::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		rotationState = 0;
		isPlacingBelt = false;
		canPlaceBelt = false;
		update();
	}

	if (event->button() == Qt::RightButton)
	{
		isDeleting = false;
	}
}

// 右键单击事件：删除的具体操作
void GameMap::rightClicked(Device* device)
{
	device->hide();
	currentMap.devices[device->getX() / GRID_SIZE][device->getY() / GRID_SIZE] = nullptr;
	device->deleteLater();
}

// R 键按下事件：旋转操作
void GameMap::keyPressEvent(QKeyEvent* event)
{
	if ((canPlaceBelt || canPlaceMiner || canPlaceCutter || canPlaceTrash) && event->key() == Qt::Key_R)
	{
		rotationState = (rotationState + 1) % 4;
		update();
		qDebug() << "Rotated to state:" << rotationState;
	}
}

// 获取删除物品位置的设备，传送给 rightClicked 函数进行具体的删除操作
void GameMap::deleteDeviceAt(const QPoint& pos)
{
	int gridX = pos.x() / GRID_SIZE;
	int gridY = pos.y() / GRID_SIZE;
	Device* device = currentMap.devices[gridX][gridY];
	if (device)
	{
		rightClicked(device);
	}
}

// 放置传送带的实现过程
void GameMap::placeBeltAt(const QPoint& pos)
{
	// 计算放置的位置，确保在格子内，然后计算最近的格子坐标（单位：像素）
	int pixelX = (pos.x() / GRID_SIZE) * GRID_SIZE;
	int pixelY = (pos.y() / GRID_SIZE) * GRID_SIZE;
	// 如果超出地图范围，则不能放置
	if (pixelX < 0 || pixelX >= WINDOW_WIDTH || pixelY < 0 || pixelY >= WINDOW_HEIGHT)
	{
		canPlaceBelt = false;
		isPlacingBelt = false;
		return;
	}
	// 如果格子里有东西，则不能放置
	if (currentMap.cycleMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.rectMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.barriers.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.hubSmall.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] != nullptr)
	{
		isEmptyGrid = false; // 该格子不为空
		return;
	}
	// 放置传送带
	Belt* newBelt = new Belt(this);
	newBelt->setPosition(pixelX, pixelY);
	newBelt->setRotationState(rotationState);
	connect(newBelt, &Belt::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newBelt; // 加入地图的 devices
	beltList.append(newBelt); // 记录传送带的指针
	qDebug() << "BeltList size: " << beltList.size();
	update();
}

// 放置开采机的实现过程
void GameMap::placeMinerAt(const QPoint& pos)
{
	// 计算放置的位置，确保在格子内，然后计算最近的格子坐标（单位：像素）
	int pixelX = (pos.x() / GRID_SIZE) * GRID_SIZE;
	int pixelY = (pos.y() / GRID_SIZE) * GRID_SIZE;
	// 如果超出地图范围，则不能放置
	if (pixelX < 0 || pixelX >= WINDOW_WIDTH || pixelY < 0 || pixelY >= WINDOW_HEIGHT)
	{
		canPlaceMiner = false;
		return;
	}
	// 如果格子里不是矿物，则不能放置开采机
	if (!maps[0].cycleMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		&& !maps[0].rectMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE)))
	{
		canPlaceMiner = false;
		rotationState = 0;
		update();
		return;
	}
	Miner* newMiner = new Miner(&currentMap.devices, this);
	newMiner->setPosition(pixelX, pixelY);
	newMiner->setRotationState(rotationState);
	connect(newMiner, &Miner::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	connect(newMiner, &Miner::newMineralGenerated, this, &GameMap::onNewMineralGenerated); // 连接生成矿物信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newMiner; // 加入地图的 devices
	canPlaceMiner = false; // 重置状态
	rotationState = 0; // 重置状态
	update();
}

// 放置切割机的实现过程
void GameMap::placeCutterAt(const QPoint& pos)
{
	// 计算放置的位置，确保在格子内，然后计算最近的格子坐标（单位：像素）
	int pixelX = (pos.x() / GRID_SIZE) * GRID_SIZE;
	int pixelY = (pos.y() / GRID_SIZE) * GRID_SIZE;
	// 如果超出地图范围，则不能放置
	if (pixelX < 0 || pixelX >= WINDOW_WIDTH || pixelY < 0 || pixelY >= WINDOW_HEIGHT)
	{
		canPlaceCutter = false;
		return;
	}
	// 如果格子里有东西，则不能放置
	if (currentMap.cycleMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.rectMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.barriers.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.hubSmall.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] != nullptr)
	{
		canPlaceCutter = false;
		rotationState = 0;
		update();
		return;
	}
	Cutter* newCutter = new Cutter(this);
	newCutter->setPosition(pixelX, pixelY);
	newCutter->setRotationState(rotationState);
	connect(newCutter, &Cutter::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newCutter; // 加入地图的 devices
	canPlaceCutter = false; // 重置状态
	rotationState = 0; // 重置状态
	update();
}

// 放置垃圾桶的实现过程
void GameMap::placeTrashAt(const QPoint& pos)
{
	// 计算放置的位置，确保在格子内，然后计算最近的格子坐标（单位：像素）
	int pixelX = (pos.x() / GRID_SIZE) * GRID_SIZE;
	int pixelY = (pos.y() / GRID_SIZE) * GRID_SIZE;
	// 如果超出地图范围，则不能放置
	if (pixelX < 0 || pixelX >= WINDOW_WIDTH || pixelY < 0 || pixelY >= WINDOW_HEIGHT)
	{
		canPlaceTrash = false;
		return;
	}
	// 如果格子里有东西，则不能放置
	if (currentMap.cycleMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.rectMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.barriers.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.hubSmall.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE))
		|| currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] != nullptr)
	{
		canPlaceTrash = false;
		rotationState = 0;
		update();
		return;
	}
	Trash* newTrash = new Trash(this);
	newTrash->setPosition(pixelX, pixelY);
	newTrash->setRotationState(rotationState);
	connect(newTrash, &Trash::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newTrash;// 加入地图的 devices
	canPlaceTrash = false; // 重置状态
	rotationState = 0; // 重置状态
	update();
}

// 实现传送带自动转弯
void GameMap::getBeltDirection(QPoint currentPosition)
{
	// 基本思路：循环检查鼠标在格子里的相对位置。假设处于格子的上边缘，接下来传送带就向上转弯，以此类推。
	// 鼠标在一个格子里的坐标（单位：像素）
	int pixelInGridX = currentPosition.x() % GRID_SIZE;
	int pixelInGridY = currentPosition.y() % GRID_SIZE;

	if (pixelInGridX <= 3 && (rotationState == _W || rotationState == _S))
	{ // 左边缘
		if (rotationState == _W)
		{
			rotationState = _W_A;
		}
		else if (rotationState == _S)
		{
			rotationState = _S_A;
		}
		rightClicked(beltList[beltList.size() - 1]); // 为了调整方向，需要先删除已经放置的传送带
		placeBeltAt(currentPosition); // 重新放置
		rotationState = _A; // rotationState 设为接下来传送带铺设的方向
		isDirectionChanged = true;
	}

	else if (pixelInGridX >= GRID_SIZE - 3 && (rotationState == _W || rotationState == _S))
	{ // 右边缘
		if (rotationState == _W)
		{
			rotationState = _W_D;
		}
		else if (rotationState == _S)
		{
			rotationState = _S_D;
		}
		rightClicked(beltList[beltList.size() - 1]);
		placeBeltAt(currentPosition);
		rotationState = _D;
		isDirectionChanged = true;
	}

	else if (pixelInGridY <= 3 && (rotationState == _D || rotationState == _A))
	{ // 上边缘
		if (rotationState == _D)
		{
			rotationState = _D_W;
		}
		else if (rotationState == _A)
		{
			rotationState = _A_W;
		}
		rightClicked(beltList[beltList.size() - 1]);
		placeBeltAt(currentPosition);
		rotationState = _W;
		isDirectionChanged = true;
	}

	else if (pixelInGridY >= GRID_SIZE - 3 && (rotationState == _D || rotationState == _A))
	{ // 下边缘
		if (rotationState == _D)
		{
			rotationState = _D_S;
		}
		else if (rotationState == _A)
		{
			rotationState = _A_S;
		}
		rightClicked(beltList[beltList.size() - 1]);
		placeBeltAt(currentPosition);
		rotationState = _S;
		isDirectionChanged = true;
	}
}

// 生成新的矿物，然后将其传送到传送带上
void GameMap::onNewMineralGenerated(Mineral* mineral)
{
	QPoint mineralGridPos(mineral->getX() / GRID_SIZE, mineral->getY() / GRID_SIZE); // 矿物所在的格子坐标
	for (Belt* belt : beltList) // 遍历传送带列表，找到矿物所在的格子
	{
		int beltGridX = belt->getX() / GRID_SIZE;
		int beltGridY = belt->getY() / GRID_SIZE;
		// 矿物生成后，开采器周围必须存在与之方向相同的传送带，才能开始运动
		if ((QPoint(beltGridX, beltGridY - 1) == mineralGridPos && mineral->getDirection() == _S
			&& (belt->getRotationState() == _S || belt->getRotationState() == _S_A || belt->getRotationState() == _S_D))
			|| (QPoint(beltGridX - 1, beltGridY) == mineralGridPos && mineral->getDirection() == _D
				&& (belt->getRotationState() == _D || belt->getRotationState() == _D_S || belt->getRotationState() == _D_W))
			|| (QPoint(beltGridX, beltGridY + 1) == mineralGridPos && mineral->getDirection() == _W
				&& (belt->getRotationState() == _W || belt->getRotationState() == _W_A || belt->getRotationState() == _W_D))
			|| (QPoint(beltGridX + 1, beltGridY) == mineralGridPos && mineral->getDirection() == _A
				&& (belt->getRotationState() == _A || belt->getRotationState() == _A_S || belt->getRotationState() == _A_W)))
		{
			mineral->startMoving();
		}
	}
}