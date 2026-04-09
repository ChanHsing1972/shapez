// Created by ChenXin.
// 游戏地图的类的实现

#include "gameMap.h"

QVector<Mineral*> GameMap::mineralList; // 初始化全局矿物对象列表

GameMap::GameMap(QWidget* parent) : QWidget(parent),
beltToPlace(12), minerToPlace(4), cutterToPlace(4), trashToPlace(4), currentTaskIndex(0)
{
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setMouseTracking(true); // 启用鼠标跟踪

	// 加载资源文件
	cycleMineImage.load("./assets/images/cycle_mine.png");
	rectMineImage.load("./assets/images/rect_mine.png");
	barrierImage.load("./assets/images/barrier.png");

	beltToPlace[_W].load("./assets/images/belt_blue_W.png");
	beltToPlace[_D].load("./assets/images/belt_blue_D.png");
	beltToPlace[_S].load("./assets/images/belt_blue_S.png");
	beltToPlace[_A].load("./assets/images/belt_blue_A.png");

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

	placeSound = new QSoundEffect(this);
	deleteSound = new QSoundEffect(this);
	chooseSound = new QSoundEffect(this);

	placeSound->setSource(QUrl::fromLocalFile("./assets/sound_effects/place.wav"));
	deleteSound->setSource(QUrl::fromLocalFile("./assets/sound_effects/delete.wav"));
	chooseSound->setSource(QUrl::fromLocalFile("./assets/sound_effects/choose.wav"));

	placeSound->setVolume(1);
	deleteSound->setVolume(1);
	chooseSound->setVolume(1);

	// 加载字体
	int fontPingFang = QFontDatabase::addApplicationFont("./PingFang-Regular.ttf");
	QString fontPingFangFamily = QFontDatabase::applicationFontFamilies(fontPingFang).at(0);
	QFont customFontPingFang(fontPingFangFamily);

	// 初始化地图，并加入地图列表，随后加载第一个地图
	MapData mapA;
	initializeMapA(mapA);
	maps.append(mapA);
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
	connect(shopButton, &QPushButton::clicked, this, [this]() {
		ShopPage* shopPage = new ShopPage(currentMap.devices, this);
		shopPage->exec();
		});
	connect(helpButton, &QPushButton::clicked, this, [this]() {
		HelpPage* helpPage = new HelpPage(this);
		helpPage->exec();
		});

	// 定时器：每隔 5min 自动保存一次
	QTimer* autoSaveTimer = new QTimer(this);
	connect(autoSaveTimer, &QTimer::timeout, this, &GameMap::autoSaveGame);
	autoSaveTimer->start(300000);

	// 初始化任务列表
	tasks.append({ "收集 10 个圆形矿物", CYCLE_MINE, 10, 0, QPixmap("./assets/images/cycle.png") });
	tasks.append({ "收集 20 个方形矿物", RECT_MINE, 20, 0, QPixmap("./assets/images/rect.png") });
	tasks.append({ "收集 50 个左半圆矿物", CYCLE_MINE_L, 50, 0, QPixmap("./assets/images/left_cycle.png") });
	tasks.append({ "收集 50 个右半圆矿物", CYCLE_MINE_R, 50, 0, QPixmap("./assets/images/right_cycle.png") });
	tasks.append({ "", CYCLE_MINE_R, INT_MAX, 0, QPixmap(0,0) });

	// 初始化任务显示
	taskIconLabel = new QLabel(this);
	taskLabel = new QLabel(this);
	taskLabel->setFont(customFontPingFang);
	taskLayout = new QHBoxLayout();
	taskLayout->addWidget(taskIconLabel);
	taskLayout->addWidget(taskLabel);
	taskLayout->setAlignment(Qt::AlignLeft);
	taskLayout->setContentsMargins(0, 0, 0, 0);
	QWidget* taskWidget = new QWidget(this);
	taskWidget->setLayout(taskLayout);
	taskWidget->setGeometry(WINDOW_WIDTH - 300, 10, 290, 100);

	updateTaskDisplay();

	completionWidget = new CompletePage(this);
	connect(completionWidget, &CompletePage::continueClicked, this, [this]() {
		completionWidget->hide();
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

	hub = new Hub(this);
	hub->setPosition((WINDOW_WIDTH / GRID_SIZE - 2) / 2 * GRID_SIZE, (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 * GRID_SIZE);
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			mapA.hubSmall.append(QPoint((WINDOW_WIDTH / GRID_SIZE - 2) / 2 + i, (WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + j));
			mapA.devices[(WINDOW_WIDTH / GRID_SIZE - 2) / 2 + i][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2 + j] = hub;
		}
	}
}

void GameMap::initializeMapB(MapData& mapB) {}

void GameMap::initializeMapC(MapData& mapB) {}

// 加载地图
void GameMap::loadMap(int level)
{
	currentMap = maps[level];
	cacheStaticMap(); // 缓存静态背景
	qDebug() << "Map updating!";
	update();
	qDebug() << "Map updated!";
}

// 缓存静态地图
void GameMap::cacheStaticMap()
{
	cachedStaticMap = QPixmap(this->size()); // 创建一个与窗口大小相同的 QPixmap
	cachedStaticMap.fill(Qt::transparent); // 填充透明背景

	QPainter painter(&cachedStaticMap);
	painter.setRenderHint(QPainter::Antialiasing);
	QColor gridColor(GRAY);

	// 绘制网格
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
}

// 绘制地图
void GameMap::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QColor gridColor(GRAY);

	// 绘制缓存的静态背景
	painter.drawPixmap(0, 0, cachedStaticMap);

	// 绘制“待放置”蓝色投影
	int mouseX = mousePosition.x() / GRID_SIZE * GRID_SIZE;
	int mouseY = mousePosition.y() / GRID_SIZE * GRID_SIZE;

	if (canPlaceBelt && placeBeltFeedback)
	{
		painter.drawPixmap(mouseX, mouseY, beltToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
	if (canPlaceMiner)
	{
		painter.drawPixmap(mouseX, mouseY, minerToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
	if (canPlaceCutter)
	{
		if (rotationState == _W || rotationState == _S)
		{
			painter.drawPixmap(mouseX, mouseY, cutterToPlace[rotationState].scaled(GRID_SIZE * 2, GRID_SIZE, Qt::KeepAspectRatio));
		}
		else if (rotationState == _D || rotationState == _A)
		{
			painter.drawPixmap(mouseX, mouseY, cutterToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE * 2, Qt::KeepAspectRatio));
		}
	}
	if (canPlaceTrash)
	{
		painter.drawPixmap(mouseX, mouseY, trashToPlace[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
	}
}

// 鼠标单击某按钮后，标记为可以放下该物品
void GameMap::onButtonClicked(QString s)
{
	chooseSound->play();
	qDebug() << s << "button clicked!";
	canPlaceBelt = (s == "Belt" ? true : false);
	placeBeltFeedback = (s == "Belt" ? true : false);
	canPlaceMiner = (s == "Miner" ? true : false);
	canPlaceCutter = (s == "Cutter" ? true : false);
	canPlaceTrash = (s == "Trash" ? true : false);
	update();
}

// 鼠标移动事件：更新光标 & 传送带拖动放置 & 拖动删除物品
void GameMap::mouseMoveEvent(QMouseEvent* event)
{
	QPoint newMousePosition = event->pos();

	// 添加边界检查，确保鼠标位置在地图范围内
	int mouseX = newMousePosition.x();
	int mouseY = newMousePosition.y();
	if (mouseX < 0) mouseX = 0;
	if (mouseY < 0) mouseY = 0;
	if (mouseX >= WINDOW_WIDTH) mouseX = WINDOW_WIDTH - 1;
	if (mouseY >= WINDOW_HEIGHT) mouseY = WINDOW_HEIGHT - 1;
	newMousePosition.setX(mouseX);
	newMousePosition.setY(mouseY);

	// 只在鼠标所在格子改变时更新，减少重绘次数
	if ((newMousePosition.x() / GRID_SIZE != mousePosition.x() / GRID_SIZE)
		|| (newMousePosition.y() / GRID_SIZE != mousePosition.y() / GRID_SIZE))
	{
		mousePosition = newMousePosition;
		if (canPlaceBelt || canPlaceMiner || canPlaceCutter || canPlaceTrash)
		{
			update();
		}

		// 拖动删除设备
		if (isDeleting)
		{
			deleteDeviceAt(event->pos()); // 删除设备
			for (auto it = mineralList.begin(); it != mineralList.end();) // 遍历矿物列表，删除设备上的矿物
			{
				Mineral* mineral = *it;
				if (mineral == nullptr) // 避免空指针异常
				{
					break;
				}
				else if (mineral != nullptr && mineral->geometry().contains(newMousePosition)) // 鼠标所在位置有矿物，删除
				{
					mineral->hide();
					mineral->update();
					mineral->deleteLater();
					it = mineralList.erase(it);
				}
				else
				{
					++it;
				}
			}
			update();
		}
	}

	// 传送带拖动放置
	if (isPlacingBelt)
	{
		placeBeltFeedback = false;
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


}

// 左/右键单击事件：放置物品/删除物品及取消选中
void GameMap::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (canPlaceBelt)
		{
			isPlacingBelt = true;
			placeBeltFeedback = true;
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

// 左/右键抬起事件：结束放置传送带/结束删除物品
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

// 右键信号槽：删除的具体操作
void GameMap::rightClicked(Device* device)
{
	// 如果是 Hub，则不删除
	if (Hub* hub = dynamic_cast<Hub*>(device))
	{
		return;
	}

	device->hide(); // 隐藏设备
	currentMap.devices[device->getX() / GRID_SIZE][device->getY() / GRID_SIZE] = nullptr; // 从地图中删除设备

	// 特殊情况：处理 Cutter 占用的第二个格子
	int gridX = device->getX() / GRID_SIZE;
	int gridY = device->getY() / GRID_SIZE;
	if (Cutter* cutter = dynamic_cast<Cutter*>(device))
	{
		if (cutter->getRotationState() == _W || cutter->getRotationState() == _S)
		{
			currentMap.devices[gridX + 1][gridY] = nullptr;
		}
		else if (cutter->getRotationState() == _D || cutter->getRotationState() == _A)
		{
			currentMap.devices[gridX][gridY + 1] = nullptr;
		}
	}

	if (isDirectionChanged == false)
	{
		deleteSound->play();
	}
	device->deleteLater();
}

// 键盘按下事件：旋转/退出/选择操作
void GameMap::keyPressEvent(QKeyEvent* event)
{
	if ((canPlaceBelt || canPlaceMiner || canPlaceCutter || canPlaceTrash) && event->key() == Qt::Key_R)
	{
		rotationState = (rotationState + 1) % 4;
		qDebug() << "Rotated to state:" << rotationState << "updating";
		update();
		qDebug() << "Rotated to state:" << rotationState << "updated";
	}

	if (event->key() == Qt::Key_Escape)
	{
		saveGame("./assets/save/save.ini");
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "退出", "当前进度已保存。确定退出游戏？",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
		{
			QApplication::quit();
		}
	}

	if (event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3 || event->key() == Qt::Key_4)
	{
		switch (event->key())
		{
		case Qt::Key_1:
			onButtonClicked("Belt");
			break;
		case Qt::Key_2:
			onButtonClicked("Miner");
			break;
		case Qt::Key_3:
			onButtonClicked("Cutter");
			break;
		case Qt::Key_4:
			onButtonClicked("Trash");
			break;
		default:
			break;
		}
	}
}

// 获取删除物品位置的设备，传送给 rightClicked 函数进行具体的删除操作
void GameMap::deleteDeviceAt(const QPoint& pos)
{
	// 计算删除的位置，确保在格子内
	if (pos.x() < 0 || pos.x() >= WINDOW_WIDTH || pos.y() < 0 || pos.y() >= WINDOW_HEIGHT)
	{
		return;
	}

	// 获取指向设备的指针，传送给 rightClicked 函数
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
	if (checkEmptyGrid(pixelX / GRID_SIZE, pixelY / GRID_SIZE) == false)
	{
		isEmptyGrid = false; // 该格子不为空
		return;
	}

	// 放置传送带
	Belt* newBelt = new Belt(this);                                       // 创建传送带对象
	placeSound->play();                                                   // 播放放置音效
	newBelt->setPosition(pixelX, pixelY);                                 // 设置传送带的位置
	newBelt->setRotationState(rotationState);                             // 设置传送带的旋转状态
	connect(newBelt, &Belt::rightClicked, this, &GameMap::rightClicked);  // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newBelt; // 加入地图的 devices
	beltList.append(newBelt);                                             // 记录传送带的指针
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
		return;
	}

	// 放置开采器
	Miner* newMiner = new Miner(&currentMap.devices, this);                          // 创建开采机对象
	placeSound->play();                                                              // 播放放置音效

	// 根据放置的位置，判断矿物类型
	if (maps[0].cycleMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE)))
	{
		newMiner->setMineralType(CYCLE_MINE);
	}
	else if (maps[0].rectMines.contains(QPoint(pixelX / GRID_SIZE, pixelY / GRID_SIZE)))
	{
		newMiner->setMineralType(RECT_MINE);
	}

	newMiner->setPosition(pixelX, pixelY);                                                 // 设置开采机的位置
	newMiner->setRotationState(rotationState);                                             // 设置开采机的旋转状态
	connect(newMiner, &Miner::rightClicked, this, &GameMap::rightClicked);                 // 连接右键点击信号
	connect(newMiner, &Miner::newMineralGenerated, this, &GameMap::onNewMineralGenerated); // 连接生成矿物信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newMiner;                 // 加入地图的 devices
	canPlaceMiner = false;                                                                 // 重置状态
	rotationState = 0;                                                                     // 重置状态
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
	if (checkEmptyGrid(pixelX / GRID_SIZE, pixelY / GRID_SIZE) == false
		|| ((rotationState == _W || rotationState == _S) && checkEmptyGrid(pixelX / GRID_SIZE + 1, pixelY / GRID_SIZE) == false)
		|| ((rotationState == _D || rotationState == _A) && checkEmptyGrid(pixelX / GRID_SIZE, pixelY / GRID_SIZE + 1) == false))
	{
		canPlaceCutter = false;
		rotationState = 0;
		return;
	}

	// 放置切割机
	Cutter* newCutter = new Cutter(&currentMap.devices, this);               // 创建切割机对象
	placeSound->play();                                                      // 播放放置音效
	newCutter->setPosition(pixelX, pixelY);                                  // 设置切割机的位置
	newCutter->setRotationState(rotationState);                              // 设置切割机的旋转状态
	connect(newCutter, &Cutter::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newCutter;  // 加入地图的 devices

	// 特殊情况：切割机占用第二个格子
	if (rotationState == _W || rotationState == _S) // 如果是横向的切割机
	{
		currentMap.devices[pixelX / GRID_SIZE + 1][pixelY / GRID_SIZE] = newCutter; // 占用第二个格子
	}
	else if (rotationState == _D || rotationState == _A) // 如果是纵向的切割机
	{
		currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE + 1] = newCutter; // 占用第二个格子
	}

	canPlaceCutter = false; // 重置状态
	rotationState = 0;      // 重置状态
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
	if (checkEmptyGrid(pixelX / GRID_SIZE, pixelY / GRID_SIZE) == false)
	{
		canPlaceTrash = false;
		rotationState = 0;
		update();
		return;
	}

	// 放置垃圾桶
	Trash* newTrash = new Trash(this);                                     // 创建垃圾桶对象
	placeSound->play();                                                    // 播放放置音效
	newTrash->setPosition(pixelX, pixelY);                                 // 设置垃圾桶的位置
	newTrash->setRotationState(rotationState);                             // 设置垃圾桶的旋转状态
	connect(newTrash, &Trash::rightClicked, this, &GameMap::rightClicked); // 连接右键点击信号
	currentMap.devices[pixelX / GRID_SIZE][pixelY / GRID_SIZE] = newTrash; // 加入地图的 devices
	canPlaceTrash = false;                                                 // 重置状态
	rotationState = 0;                                                     // 重置状态
	update();
}

// 实现传送带自动转弯
void GameMap::getBeltDirection(QPoint currentPosition)
{
	// 基本思路：循环检查鼠标在格子里的相对位置。假设处于格子的上边缘，接下来传送带就向上转弯，以此类推。
	// 鼠标在一个格子里的坐标（单位：像素）
	int pixelInGridX = currentPosition.x() % GRID_SIZE;
	int pixelInGridY = currentPosition.y() % GRID_SIZE;

	// 确保像素坐标在有效范围内
	if (pixelInGridX < 0 || pixelInGridX >= GRID_SIZE || pixelInGridY < 0 || pixelInGridY >= GRID_SIZE)
	{
		return;
	}

	// 左边缘，向左转弯
	if (pixelInGridX <= EDGE_SIZE && (rotationState == _W || rotationState == _S))
	{
		if (rotationState == _W)
		{
			rotationState = _W_A;
		}
		else if (rotationState == _S)
		{
			rotationState = _S_A;
		}
		isDirectionChanged = true;                   // 方向改变
		rightClicked(beltList[beltList.size() - 1]); // 为了调整方向，需要先删除已经放置的传送带
		placeBeltAt(currentPosition);                // 重新放置
		rotationState = _A;                          // rotationState 设为接下来传送带铺设的方向
	}

	// 右边缘，向右转弯
	else if (pixelInGridX >= GRID_SIZE - EDGE_SIZE && (rotationState == _W || rotationState == _S))
	{
		if (rotationState == _W)
		{
			rotationState = _W_D;
		}
		else if (rotationState == _S)
		{
			rotationState = _S_D;
		}
		isDirectionChanged = true;									 // 方向改变
		rightClicked(beltList[beltList.size() - 1]); // 为了调整方向，需要先删除已经放置的传送带
		placeBeltAt(currentPosition);								 // 重新放置
		rotationState = _D;													 // rotationState 设为接下来传送带铺设的方向
	}

	// 上边缘，向上转弯
	else if (pixelInGridY <= EDGE_SIZE && (rotationState == _D || rotationState == _A))
	{
		if (rotationState == _D)
		{
			rotationState = _D_W;
		}
		else if (rotationState == _A)
		{
			rotationState = _A_W;
		}
		isDirectionChanged = true;                   // 方向改变
		rightClicked(beltList[beltList.size() - 1]); // 为了调整方向，需要先删除已经放置的传送带
		placeBeltAt(currentPosition);								 // 重新放置
		rotationState = _W;													 // rotationState 设为接下来传送带铺设的方向
	}

	// 下边缘，向下转弯
	else if (pixelInGridY >= GRID_SIZE - EDGE_SIZE && (rotationState == _D || rotationState == _A))
	{
		if (rotationState == _D)
		{
			rotationState = _D_S;
		}
		else if (rotationState == _A)
		{
			rotationState = _A_S;
		}
		isDirectionChanged = true;                   // 方向改变
		rightClicked(beltList[beltList.size() - 1]); // 为了调整方向，需要先删除已经放置的传送带
		placeBeltAt(currentPosition);								 // 重新放置
		rotationState = _S;													 // rotationState 设为接下来传送带铺设的方向
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

// 检查格子是否为空
bool GameMap::checkEmptyGrid(int gridX, int gridY)
{
	if (currentMap.cycleMines.contains(QPoint(gridX, gridY))
		|| currentMap.rectMines.contains(QPoint(gridX, gridY))
		|| currentMap.barriers.contains(QPoint(gridX, gridY))
		|| currentMap.hubSmall.contains(QPoint(gridX, gridY))
		|| currentMap.devices[gridX][gridY] != nullptr)
	{
		return false;
	}
	return true;
}

// 控制淡入动画
void GameMap::startFadeInAnimation()
{
	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	this->setGraphicsEffect(opacityEffect);
	QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
	animation->setDuration(300);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// 保存游戏状态到 INI 文件
void GameMap::saveGame(const QString& fileName)
{
	QSettings settings(fileName, QSettings::IniFormat);

	// 保存矿物信息
	settings.beginGroup("Mines");
	settings.setValue("cycleMines", QVariant::fromValue(currentMap.cycleMines));
	settings.setValue("rectMines", QVariant::fromValue(currentMap.rectMines));
	settings.endGroup();

	// 保存障碍信息
	settings.beginGroup("Barriers");
	settings.setValue("barriers", QVariant::fromValue(currentMap.barriers));
	settings.endGroup();

	// 保存 Hub 信息
	settings.beginGroup("Hub");
	settings.setValue("hubSmall", QVariant::fromValue(currentMap.hubSmall));
	settings.setValue("hubMineralCount", hub->getMineralCount());
	settings.endGroup();

	// 保存商店等级和设备相关属性
	settings.beginGroup("Shop");
	settings.setValue("minerLevel", minerLevel);
	settings.setValue("beltLevel", beltLevel);
	settings.setValue("cutterLevel", cutterLevel);
	settings.setValue("beltCost", beltCost);
	settings.setValue("minerCost", minerCost);
	settings.setValue("cutterCost", cutterCost);
	settings.setValue("beltSpeed", BELT_SPEED);
	settings.setValue("minerSpeed", MINER_SPEED);
	settings.setValue("cutterSpeed", CUTTER_SPEED);
	settings.setValue("taskIndex", currentTaskIndex);
	settings.endGroup();

	// 保存设备信息
	settings.beginGroup("Devices");
	for (int i = 0; i < currentMap.devices.size(); ++i) {
		for (int j = 0; j < currentMap.devices[i].size(); ++j) {
			if (currentMap.devices[i][j] != nullptr) {
				settings.setValue(QString("device_%1_%2_type").arg(i).arg(j), currentMap.devices[i][j]->getTypeID()); // 设备类型 ID
				settings.setValue(QString("device_%1_%2_rotation").arg(i).arg(j), currentMap.devices[i][j]->getRotationState()); // 设备旋转状态
				qDebug() << "Device" << currentMap.devices[i][j]->getTypeID() << "saved at" << i << j;
			}
			else {
				settings.setValue(QString("device_%1_%2_type").arg(i).arg(j), -1); // 空设备用 -1 表示
				settings.setValue(QString("device_%1_%2_rotation").arg(i).arg(j), -1); // 空设备用 -1 表示
			}
		}
	}
	settings.endGroup();
}

// 从 INI 文件加载游戏状态
void GameMap::loadGame(const QString& fileName)
{
	qDebug() << "Loading game from" << fileName;
	QSettings settings(fileName, QSettings::IniFormat);

	// 加载矿物信息
	settings.beginGroup("Mines");
	currentMap.cycleMines = settings.value("cycleMines").value<QVector<QPoint>>();
	currentMap.rectMines = settings.value("rectMines").value<QVector<QPoint>>();
	settings.endGroup();
	qDebug() << "Mines loaded successfully!";

	// 加载障碍信息
	settings.beginGroup("Barriers");
	currentMap.barriers = settings.value("barriers").value<QVector<QPoint>>();
	settings.endGroup();
	qDebug() << "Barriers loaded successfully!";

	// 加载 Hub 信息
	settings.beginGroup("Hub");
	currentMap.hubSmall = settings.value("hubSmall").value<QVector<QPoint>>();
	hub->setMineralCount(settings.value("hubMineralCount").toInt());
	settings.endGroup();
	qDebug() << "Hub loaded successfully!";

	// 加载商店等级和设备相关属性
	settings.beginGroup("Shop");
	minerLevel = settings.value("minerLevel").toInt();
	beltLevel = settings.value("beltLevel").toInt();
	cutterLevel = settings.value("cutterLevel").toInt();
	beltCost = settings.value("beltCost").toInt();
	minerCost = settings.value("minerCost").toInt();
	cutterCost = settings.value("cutterCost").toInt();
	BELT_SPEED = settings.value("beltSpeed").toInt();
	MINER_SPEED = settings.value("minerSpeed").toInt();
	CUTTER_SPEED = settings.value("cutterSpeed").toInt();
	currentTaskIndex = settings.value("taskIndex").toInt();
	settings.endGroup();
	qDebug() << "Shop loaded successfully!";

	// 加载设备
	settings.beginGroup("Devices");
	for (int i = 0; i < currentMap.devices.size(); ++i)
	{
		for (int j = 0; j < currentMap.devices[i].size(); ++j)
		{
			int typeID = settings.value(QString("device_%1_%2_type").arg(i).arg(j), -1).toInt();
			rotationState = settings.value(QString("device_%1_%2_rotation").arg(i).arg(j), -1).toInt();
			if (typeID != -1)
			{
				// 根据类型 ID 创建设备并放置到地图上
				createDeviceByTypeID(typeID, i, j);
				qDebug() << "Device" << typeID << "loaded at" << i << j;
			}
		}
	}
	settings.endGroup();
	qDebug() << "Devices loaded successfully!";

	cacheStaticMap();
	update();
	qDebug() << "Game loaded successfully!";

	// 模拟一次鼠标点击事件
	// （因为有个 bug 难以修复，但是我发现加载后点击一次鼠标可以莫名其妙地解决这个 bug……所以这里模拟一次鼠标点击 /笑）
	QTimer::singleShot(100, this, [this]() {
		QPoint clickPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		QMouseEvent* clickEvent = new QMouseEvent(QEvent::MouseButtonPress, clickPosition, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		QCoreApplication::postEvent(this, clickEvent);
		QMouseEvent* releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, clickPosition, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
		QCoreApplication::postEvent(this, releaseEvent);
		});
}

// 根据类型 ID 创建设备
void GameMap::createDeviceByTypeID(int typeID, int i, int j)
{
	switch (typeID)
	{
	case 0: placeBeltAt(QPoint(i * GRID_SIZE, j * GRID_SIZE)); break;
	case 1: placeMinerAt(QPoint(i * GRID_SIZE, j * GRID_SIZE)); break;
	case 2: placeCutterAt(QPoint(i * GRID_SIZE, j * GRID_SIZE)); break;
	case 3: placeTrashAt(QPoint(i * GRID_SIZE, j * GRID_SIZE)); break;
	default: break;
	}
}

// 自动保存游戏
void GameMap::autoSaveGame()
{
	QString fileName = "./assets/save/save.ini";
	saveGame(fileName);
}

// 更新任务进度
void GameMap::updateTaskProgress(QVector<int>& collectedMines)
{
	tasks[currentTaskIndex].currentCount = collectedMines[tasks[currentTaskIndex].mineralType];
	updateTaskDisplay();
	if (tasks[currentTaskIndex].currentCount >= tasks[currentTaskIndex].targetCount)
	{
		if (currentTaskIndex < tasks.size() - 1)
		{
			completionWidget->show();
			currentTaskIndex++;
		}
	}
}

// 更新任务显示
void GameMap::updateTaskDisplay()
{
	const Task& currentTask = tasks[currentTaskIndex];
	if (currentTaskIndex == tasks.size() - 1) // 如果完成了最后一个任务
	{
		taskLabel->setText(QString("")); // 不再显示任务信息
		taskIconLabel->setPixmap(currentTask.icon); // 任务图标为空
		return;
	}
	taskLabel->setText(QString("%1<br>当前进度：%2/%3").arg(currentTask.description).arg(currentTask.currentCount).arg(currentTask.targetCount));
	taskIconLabel->setPixmap(currentTask.icon.scaled(40, 40, Qt::KeepAspectRatio));
}