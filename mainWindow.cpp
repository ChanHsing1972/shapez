#include "mainWindow.h"

mainWindow::mainWindow(QWidget* parent) : QWidget(parent)
{
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setWindowIcon(QIcon("./assets/images/icon.png"));

	// QStackedWidget: 管理页面
	QStackedWidget* stackedWidget = new QStackedWidget(this);
	WelcomePage* welcomePage = new WelcomePage(this);
	GameMap* gameMap = new GameMap(this);

	// 将页面添加到 QStackedWidget
	stackedWidget->addWidget(welcomePage);
	stackedWidget->addWidget(gameMap);

	// 将 stackedWidget 设置为 MainWindow 的子部件
	stackedWidget->setParent(this);
	stackedWidget->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// 连接欢迎页面的信号到地图页面的显示
	connect(welcomePage, &WelcomePage::startGame, [stackedWidget]() {
		stackedWidget->setCurrentWidget(stackedWidget->widget(1));
		});

	// 显示欢迎页面
	stackedWidget->setCurrentWidget(welcomePage);
}

mainWindow::~mainWindow()
{
}

