#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent) : QWidget(parent)
{
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setWindowIcon(QIcon("./assets/images/icon.png"));

	// QStackedWidget: 管理页面
	stackedWidget = new QStackedWidget(this);
	welcomePage = new WelcomePage(this);
	gameMap = new GameMap(this);

	// 将页面添加到 QStackedWidget
	stackedWidget->addWidget(welcomePage);
	stackedWidget->addWidget(gameMap);

	// 将 stackedWidget 设置为 MainWindow 的子部件
	stackedWidget->setParent(this);
	stackedWidget->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// 连接开始按钮到地图页面的切换
	connect(welcomePage, &WelcomePage::startGame, [this]() {
		stackedWidget->setCurrentWidget(gameMap);
		});

	// 连接加载按钮到地图页面的切换
	connect(welcomePage, &WelcomePage::loadGame, this, [this](const QString& fileName) {
		stackedWidget->setCurrentWidget(gameMap);
		gameMap->loadGame(fileName);
		});

	// 当接收到 startGameFadeIn 信号时，启动 GameMap 的淡入效果
	connect(welcomePage, &WelcomePage::startGame, gameMap, &GameMap::startFadeInAnimation);

	// 显示欢迎页面
	stackedWidget->setCurrentWidget(welcomePage);
}

mainWindow::~mainWindow()
{
	delete gameMap;
	delete welcomePage;
	delete stackedWidget;
}

// 重写关闭时事件，用于在关闭窗口时保存游戏
void mainWindow::closeEvent(QCloseEvent* event)
{
	gameMap->saveGame("./assets/save/save.ini");
	event->accept();
}