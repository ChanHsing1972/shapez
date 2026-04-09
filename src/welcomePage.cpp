// Created by Chenxin.
// 欢迎界面类的实现。

#include "welcomePage.h"
#include <QFontDatabase>

WelcomePage::WelcomePage(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("Welcome to the Shapez");
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	QVBoxLayout* layout = new QVBoxLayout(this);

	startButton = new QPushButton(this);
	loadButton = new QPushButton(this);
	exitButton = new QPushButton(this);

	startButton->setFixedSize(270, 60);
	loadButton->setFixedSize(270, 60);
	exitButton->setFixedSize(270, 60);

	startButton->setText("新的游戏");
	loadButton->setText("继续游戏");
	exitButton->setText("退      出");

	int fontPingFang = QFontDatabase::addApplicationFont("./PingFang-Regular.ttf");
	QString fontPingFangFamily = QFontDatabase::applicationFontFamilies(fontPingFang).at(0);
	QFont customFontPingFang(fontPingFangFamily);

	startButton->setFont(customFontPingFang);
	loadButton->setFont(customFontPingFang);
	exitButton->setFont(customFontPingFang);

	startButton->setStyleSheet(R"(
        QPushButton {
            font-size: 28px;
            background-color: #008700;
            color: white;
            border-radius: 25px;
        }
        QPushButton:hover {
            background-color: #008700;
        }
        QPushButton:pressed {
            background-color: #005500;
            padding-left: 3px;
            padding-top: 3px;
        }
    )");

	loadButton->setStyleSheet(R"(
				QPushButton {
						font-size: 28px;
						background-color: #0045FF;
						color: white;
						border-radius: 25px;
				}
				QPushButton:hover {
						background-color: #0045FF;
				}
				QPushButton:pressed {
						background-color: #002A99;
						padding-left: 3px;
						padding-top: 3px;
				}
		)");

	exitButton->setStyleSheet(R"(
        QPushButton {
            font-size: 28px;
            background-color: #505050;
            color: white;
            border-radius: 25px;
        }
        QPushButton:hover {
            background-color: #505050;
        }
        QPushButton:pressed {
            background-color: #404040;
            padding-left: 3px;
            padding-top: 3px;
        }
    )");

	layout->setSpacing(20); // 按钮之间的间距
	layout->setContentsMargins(10, 10, 10, 10); // 窗口边缘的间距

	// 添加按钮到布局
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 上方空间
	layout->addWidget(startButton, 0, Qt::AlignHCenter);
	layout->addWidget(loadButton, 0, Qt::AlignCenter);
	layout->addWidget(exitButton, 0, Qt::AlignHCenter);
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 下方空间

	connect(startButton, &QPushButton::clicked, this, &WelcomePage::onStartClicked);
	connect(loadButton, &QPushButton::clicked, this, &WelcomePage::onLoadClicked);
	connect(exitButton, &QPushButton::clicked, this, &WelcomePage::onExitClicked);

	// 为按钮启用鼠标悬停事件
	startButton->setAttribute(Qt::WA_Hover, true);
	loadButton->setAttribute(Qt::WA_Hover, true);
	exitButton->setAttribute(Qt::WA_Hover, true);

	// 为按钮添加透明效果
	QGraphicsOpacityEffect* startOpacityEffect = new QGraphicsOpacityEffect(startButton);
	startButton->setGraphicsEffect(startOpacityEffect);

	QGraphicsOpacityEffect* continueOpacityEffect = new QGraphicsOpacityEffect(loadButton);
	loadButton->setGraphicsEffect(continueOpacityEffect);

	QGraphicsOpacityEffect* exitOpacityEffect = new QGraphicsOpacityEffect(exitButton);
	exitButton->setGraphicsEffect(exitOpacityEffect);

	// 将透明效果作为属性保存
	startButton->setProperty("opacityEffect", QVariant::fromValue(startOpacityEffect));
	loadButton->setProperty("opacityEffect", QVariant::fromValue(continueOpacityEffect));
	exitButton->setProperty("opacityEffect", QVariant::fromValue(exitOpacityEffect));

	// 安装事件过滤器
	startButton->installEventFilter(this);
	loadButton->installEventFilter(this);
	exitButton->installEventFilter(this);

	bgmPlayer = new QMediaPlayer(this);
	bgmPlaylist = new QMediaPlaylist(this);
	bgmPlaylist->addMedia(QUrl("./assets/music/theme-full.mp3"));
	bgmPlaylist->setPlaybackMode(QMediaPlaylist::Loop); // 循环播放
	bgmPlayer->setPlaylist(bgmPlaylist);
	bgmPlayer->setVolume(80); // 设置音量
	bgmPlayer->play();
}

bool WelcomePage::eventFilter(QObject* watched, QEvent* event)
{
	if ((watched == startButton || watched == loadButton || watched == exitButton) && event->type() == QEvent::Enter)
	{
		// 鼠标进入按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(0.7);
		animation->setEndValue(1.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else if ((watched == startButton || watched == loadButton || watched == exitButton) && event->type() == QEvent::Leave)
	{
		// 鼠标离开按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(1.0);
		animation->setEndValue(0.7);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}

	return QWidget::eventFilter(watched, event);
}

void WelcomePage::onStartClicked()
{
	// 移除按钮的透明度效果
	startButton->setGraphicsEffect(nullptr);
	loadButton->setGraphicsEffect(nullptr);
	exitButton->setGraphicsEffect(nullptr);

	// 移除按钮的事件过滤器
	startButton->removeEventFilter(this);
	loadButton->removeEventFilter(this);
	exitButton->removeEventFilter(this);

	// 创建淡出效果
	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	this->setGraphicsEffect(opacityEffect);
	QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
	animation->setDuration(300); // 设置淡出持续时间（毫秒）
	animation->setStartValue(1.0);
	animation->setEndValue(0.0);
	animation->start(QAbstractAnimation::DeleteWhenStopped);

	// 当淡出动画完成后，隐藏当前页面并发送信号
	connect(animation, &QPropertyAnimation::finished, this, [this]() {
		emit startGame(); // 发出开始游戏信号
		});
}

void WelcomePage::onLoadClicked()
{
	emit loadGame("./assets/save/save.ini");
}

void WelcomePage::onExitClicked()
{
	QApplication::quit(); // 退出应用程序
}