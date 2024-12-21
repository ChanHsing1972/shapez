#include "welcomePage.h"

WelcomePage::WelcomePage(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("Welcome to the Game");
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	QVBoxLayout* layout = new QVBoxLayout(this);

	QPushButton* startButton = new QPushButton(this);
	QPushButton* exitButton = new QPushButton(this);

	startButton->setFixedSize(200, 50);
	exitButton->setFixedSize(200, 50);

	startButton->setText("开始游戏");
	exitButton->setText("退出");

	startButton->setFont(QFont("等线"));
	exitButton->setFont(QFont("等线"));

	startButton->setStyleSheet(
		"QPushButton {"
		"   font-size: 24px;"
		"   background-color: #1F1F1F;"
		"   color: white;"
		"   border-radius: 15px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #3A3A3A;"
		"}"
		"QPushButton:pressed {"
		"   background-color: #575757;"
		"   padding-left: 3px;"
		"   padding-top: 3px;"
		"}");
	exitButton->setStyleSheet(
		"QPushButton {"
		"   font-size: 24px;"
		"   background-color: #1F1F1F;"
		"   color: white;"
		"   border-radius: 15px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #3A3A3A;"
		"}"
		"QPushButton:pressed {"
		"   background-color: #575757;"
		"   padding-left: 3px;"
		"   padding-top: 3px;"
		"}");

	layout->setSpacing(20); // 按钮之间的间距
	layout->setContentsMargins(10, 10, 10, 10); // 窗口边缘的间距

	// 添加按钮到布局
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 上方空间
	layout->addWidget(startButton, 0, Qt::AlignHCenter); // 中心对齐
	layout->addWidget(exitButton, 0, Qt::AlignHCenter); // 中心对齐
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 下方空间

	connect(startButton, &QPushButton::clicked, this, &WelcomePage::onStartClicked);
	connect(exitButton, &QPushButton::clicked, this, &WelcomePage::onExitClicked);
}

void WelcomePage::onStartClicked()
{
	emit startGame(); // 发出开始游戏信号
}

void WelcomePage::onExitClicked()
{
	QApplication::quit(); // 退出应用程序
}