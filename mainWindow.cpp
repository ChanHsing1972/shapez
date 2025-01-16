// Created by ChenXin.
// 主窗口类的实现。

#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent) : QWidget(parent)
{
	// 获取当前屏幕的分辨率
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	WINDOW_WIDTH = screenGeometry.width();
	WINDOW_HEIGHT = screenGeometry.height();

	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setWindowIcon(QIcon("./assets/images/icon.png"));

	// 创建页面
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

	// 全屏显示欢迎页面
	stackedWidget->setCurrentWidget(welcomePage);
	showFullScreen();
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

// 重写键盘按下事件，用于处理 Esc 键退出游戏
void mainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "退出", "确定退出游戏？",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes)
		{
			QApplication::quit();
		}
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}