#include "welcomePage.h"
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QEvent>

WelcomePage::WelcomePage(QWidget* parent) : QWidget(parent)
{
	setWindowTitle("Welcome to the Shapez");
	setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	QVBoxLayout* layout = new QVBoxLayout(this);

	startButton = new QPushButton(this);
	exitButton = new QPushButton(this);

	startButton->setFixedSize(270, 60);
	exitButton->setFixedSize(270, 60);

	startButton->setText("开始游戏");
	exitButton->setText("退出");

	startButton->setFont(QFont("等线"));
	exitButton->setFont(QFont("等线"));

	// 设置按钮样式（移除了不支持的属性）
	QString buttonStyle = R"(
        QPushButton {
            font-size: 28px;
            background-color: #2C9E29;
            color: white;
            border-radius: 25px;
        }
        QPushButton:hover {
            background-color: #2C9E29;
        }
        QPushButton:pressed {
            background-color: #385A40;
            padding-left: 3px;
            padding-top: 3px;
        }
    )";

	startButton->setStyleSheet(buttonStyle);
	exitButton->setStyleSheet(buttonStyle);

	layout->setSpacing(20); // 按钮之间的间距
	layout->setContentsMargins(10, 10, 10, 10); // 窗口边缘的间距

	// 添加按钮到布局
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 上方空间
	layout->addWidget(startButton, 0, Qt::AlignHCenter); // 中心对齐
	layout->addWidget(exitButton, 0, Qt::AlignHCenter); // 中心对齐
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)); // 下方空间

	connect(startButton, &QPushButton::clicked, this, &WelcomePage::onStartClicked);
	connect(exitButton, &QPushButton::clicked, this, &WelcomePage::onExitClicked);

	// 为按钮启用鼠标悬停事件
	startButton->setAttribute(Qt::WA_Hover, true);
	exitButton->setAttribute(Qt::WA_Hover, true);

	// 为按钮添加透明效果
	QGraphicsOpacityEffect* startOpacityEffect = new QGraphicsOpacityEffect(startButton);
	startButton->setGraphicsEffect(startOpacityEffect);

	QGraphicsOpacityEffect* exitOpacityEffect = new QGraphicsOpacityEffect(exitButton);
	exitButton->setGraphicsEffect(exitOpacityEffect);

	// 将透明效果作为属性保存
	startButton->setProperty("opacityEffect", QVariant::fromValue(startOpacityEffect));
	exitButton->setProperty("opacityEffect", QVariant::fromValue(exitOpacityEffect));

	// 安装事件过滤器
	startButton->installEventFilter(this);
	exitButton->installEventFilter(this);
}

bool WelcomePage::eventFilter(QObject* watched, QEvent* event)
{
	if ((watched == startButton || watched == exitButton) && event->type() == QEvent::Enter)
	{
		// 鼠标进入按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(0.7);
		animation->setEndValue(1.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else if ((watched == startButton || watched == exitButton) && event->type() == QEvent::Leave)
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
	emit startGame(); // 发出开始游戏信号
}

void WelcomePage::onExitClicked()
{
	QApplication::quit(); // 退出应用程序
}