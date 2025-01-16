// Created by Chenxin.
// 商店页面类的实现。

#include "shopPage.h"

#define BUTTON_WIDTH 180
#define BUTTON_HEIGHT 60

ShopPage::ShopPage(QVector<QVector<Device*>>& devices, QWidget* parent)
	: QDialog(parent), devices(devices)
{
	setWindowTitle("商店");
	setFixedSize(WINDOW_WIDTH * 2 / 3, WINDOW_HEIGHT * 2 / 3);
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	int fontPingFang = QFontDatabase::addApplicationFont("./PingFang-Regular.ttf");
	QString fontPingFangFamily = QFontDatabase::applicationFontFamilies(fontPingFang).at(0);
	QFont customFontPingFang(fontPingFangFamily);

	int fontYaHei = QFontDatabase::addApplicationFont("./微软雅黑.ttc");
	QString fontYaHeiFamily = QFontDatabase::applicationFontFamilies(fontYaHei).at(0);
	QFont customFontYaHei(fontYaHeiFamily);

	QString buttonStyleSheet =
		R"(
        QPushButton {
            font-size: 28px;
            background-color: #008700;
            color: white;
            border-radius: 20px;
        }
        QPushButton:hover {
            background-color: #008700;
        }
        QPushButton:pressed {
            background-color: #297E33;
            padding-left: 3px;
            padding-top: 3px;
        }
    )";
	QString labelStyleSheet = "font-size: 44px; color: #666870;";
	QString costLabelStyleSheet = "font-size: 28px; color: #000000;";

	// 开采器布局
	QHBoxLayout* minerLayout = new QHBoxLayout();
	minerLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	QLabel* minerIcon = new QLabel(this);
	minerIcon->setPixmap(QPixmap("./assets/images/miner_button.png").scaled(50, 50, Qt::KeepAspectRatio));
	QLabel* minerLabel = new QLabel("<h4>开采器</h4>", this);
	minerLabel->setFont(customFontYaHei);
	minerLabel->setStyleSheet(labelStyleSheet);
	minerLevelLabel = new QLabel(QString("等级 %1").arg(minerLevel), this);
	minerLevelLabel->setFont(customFontYaHei);
	minerLevelLabel->setStyleSheet(costLabelStyleSheet);
	minerCostLabel = new QLabel(QString("消耗 %1 摩拉").arg(minerCost), this);
	minerCostLabel->setFont(customFontYaHei);
	minerCostLabel->setStyleSheet(costLabelStyleSheet);
	minerButton = new QPushButton("升级", this);
	minerButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	minerButton->setFont(customFontPingFang);
	minerButton->setStyleSheet(buttonStyleSheet);
	minerLayout->addWidget(minerIcon);
	minerLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	minerLayout->addWidget(minerLabel);
	minerLayout->addSpacerItem(new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	minerLayout->addWidget(minerLevelLabel);
	minerLayout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	minerLayout->addWidget(minerCostLabel);
	minerLayout->addSpacerItem(new QSpacerItem(250, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	minerLayout->addWidget(minerButton);
	minerLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));


	// 传送带布局
	QHBoxLayout* beltLayout = new QHBoxLayout();
	beltLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	QLabel* beltIcon = new QLabel(this);
	beltIcon->setPixmap(QPixmap("./assets/images/belt_button.png").scaled(50, 50, Qt::KeepAspectRatio));
	QLabel* beltLabel = new QLabel("<h4>传送带</h4>", this);
	beltLabel->setFont(customFontYaHei);
	beltLabel->setStyleSheet(labelStyleSheet);
	beltLevelLabel = new QLabel(QString("等级 %1").arg(beltLevel), this);
	beltLevelLabel->setFont(customFontYaHei);
	beltLevelLabel->setStyleSheet(costLabelStyleSheet);
	beltCostLabel = new QLabel(QString("消耗 %1 摩拉").arg(beltCost), this);
	beltCostLabel->setFont(customFontYaHei);
	beltCostLabel->setStyleSheet(costLabelStyleSheet);
	beltButton = new QPushButton("升级", this);
	beltButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	beltButton->setFont(customFontPingFang);
	beltButton->setStyleSheet(buttonStyleSheet);
	beltLayout->addWidget(beltIcon);
	beltLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	beltLayout->addWidget(beltLabel);
	beltLayout->addSpacerItem(new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	beltLayout->addWidget(beltLevelLabel);
	beltLayout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	beltLayout->addWidget(beltCostLabel);
	beltLayout->addSpacerItem(new QSpacerItem(250, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	beltLayout->addWidget(beltButton);
	beltLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	// 切割机布局
	QHBoxLayout* cutterLayout = new QHBoxLayout();
	cutterLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	QLabel* cutterIcon = new QLabel(this);
	cutterIcon->setPixmap(QPixmap("./assets/images/cutter_button.png").scaled(50, 50, Qt::KeepAspectRatio));
	QLabel* cutterLabel = new QLabel("<h4>切割机</h4>", this);
	cutterLabel->setFont(customFontYaHei);
	cutterLabel->setStyleSheet(labelStyleSheet);
	cutterLevelLabel = new QLabel(QString("等级 %1").arg(cutterLevel), this);
	cutterLevelLabel->setFont(customFontYaHei);
	cutterLevelLabel->setStyleSheet(costLabelStyleSheet);
	cutterCostLabel = new QLabel(QString("消耗 %1 摩拉").arg(cutterCost), this);
	cutterCostLabel->setFont(customFontYaHei);
	cutterCostLabel->setStyleSheet(costLabelStyleSheet);
	cutterButton = new QPushButton("升级", this);
	cutterButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	cutterButton->setFont(customFontPingFang);
	cutterButton->setStyleSheet(buttonStyleSheet);
	cutterLayout->addWidget(cutterIcon);
	cutterLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	cutterLayout->addWidget(cutterLabel);
	cutterLayout->addSpacerItem(new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	cutterLayout->addWidget(cutterLevelLabel);
	cutterLayout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	cutterLayout->addWidget(cutterCostLabel);
	cutterLayout->addSpacerItem(new QSpacerItem(250, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
	cutterLayout->addWidget(cutterButton);
	cutterLayout->addSpacerItem(new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	// 将各个布局添加到主布局中
	mainLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Fixed, QSizePolicy::Minimum));
	mainLayout->addLayout(minerLayout);
	mainLayout->addLayout(beltLayout);
	mainLayout->addLayout(cutterLayout);
	mainLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Fixed, QSizePolicy::Minimum));

	connect(minerButton, &QPushButton::clicked, this, &ShopPage::applyMinerSpeed);
	connect(beltButton, &QPushButton::clicked, this, &ShopPage::applyBeltSpeed);
	connect(cutterButton, &QPushButton::clicked, this, &ShopPage::applyCutterSpeed);

	// 为按钮启用鼠标悬停事件
	minerButton->setAttribute(Qt::WA_Hover, true);
	beltButton->setAttribute(Qt::WA_Hover, true);
	cutterButton->setAttribute(Qt::WA_Hover, true);

	// 为按钮添加透明效果
	QGraphicsOpacityEffect* minerOpacityEffect = new QGraphicsOpacityEffect(minerButton);
	minerButton->setGraphicsEffect(minerOpacityEffect);

	QGraphicsOpacityEffect* beltOpacityEffect = new QGraphicsOpacityEffect(beltButton);
	beltButton->setGraphicsEffect(beltOpacityEffect);

	QGraphicsOpacityEffect* cutterOpacityEffect = new QGraphicsOpacityEffect(beltButton);
	cutterButton->setGraphicsEffect(cutterOpacityEffect);

	// 将透明效果作为属性保存
	minerButton->setProperty("opacityEffect", QVariant::fromValue(minerOpacityEffect));
	beltButton->setProperty("opacityEffect", QVariant::fromValue(beltOpacityEffect));
	cutterButton->setProperty("opacityEffect", QVariant::fromValue(cutterOpacityEffect));

	// 安装事件过滤器
	minerButton->installEventFilter(this);
	beltButton->installEventFilter(this);
	cutterButton->installEventFilter(this);
}

void ShopPage::applyMinerSpeed()
{
	if (Hub* hub = dynamic_cast<Hub*>((devices)[(WINDOW_WIDTH / GRID_SIZE - 2) / 2][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2]))
	{
		if (hub->getMineralCount() < 10)
		{
			QMessageBox::warning(this, "失败", "矿物数量不足，无法修改开采器速度。");
			return;
		}
		if (MINER_SPEED <= 1200)
		{
			QMessageBox::warning(this, "失败", "已达到最大开采速度。");
			return;
		}
		hub->reduceMineralCount(minerCost);
	}
	MINER_SPEED /= 2;

	minerLevel++;
	minerCost *= 2;
	minerLevelLabel->setText(QString("等级 %1").arg(minerLevel));
	minerCostLabel->setText(QString("消耗 %1 摩拉").arg(minerCost));

	QMessageBox::information(this, "成功", "已修改开采器速度。");
}

void ShopPage::applyBeltSpeed()
{
	if (Hub* hub = dynamic_cast<Hub*>((devices)[(WINDOW_WIDTH / GRID_SIZE - 2) / 2][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2]))
	{
		if (hub->getMineralCount() < 15)
		{
			QMessageBox::warning(this, "失败", "矿物数量不足，无法修改传送带速度。");
			return;
		}
		if (BELT_SPEED >= 3)
		{
			QMessageBox::warning(this, "失败", "已达到最大传送速度。");
			return;
		}
		hub->reduceMineralCount(beltCost);
	}
	BELT_SPEED++;

	beltLevel++;
	beltCost *= 2;
	beltLevelLabel->setText(QString("等级 %1").arg(beltLevel));
	beltCostLabel->setText(QString("消耗 %1 摩拉").arg(beltCost));

	QMessageBox::information(this, "成功", "已修改传送带速度。");
}

void ShopPage::applyCutterSpeed()
{
	if (Hub* hub = dynamic_cast<Hub*>((devices)[(WINDOW_WIDTH / GRID_SIZE - 2) / 2][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2]))
	{
		if (hub->getMineralCount() < 5)
		{
			QMessageBox::warning(this, "失败", "矿物数量不足，无法修改切割机速度。");
			return;
		}
		if (CUTTER_SPEED <= 125)
		{
			QMessageBox::warning(this, "失败", "已达到最大切割速度。");
			return;
		}
		hub->reduceMineralCount(cutterCost);
	}
	CUTTER_SPEED /= 4;

	cutterLevel++;
	cutterCost *= 2;
	cutterLevelLabel->setText(QString("等级 %1").arg(cutterLevel));
	cutterCostLabel->setText(QString("消耗 %1 摩拉").arg(cutterCost));

	QMessageBox::information(this, "成功", "已修改切割机速度。");
}

bool ShopPage::eventFilter(QObject* watched, QEvent* event)
{
	if ((watched == minerButton || watched == beltButton || watched == cutterButton) && event->type() == QEvent::Enter)
	{
		// 鼠标进入按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(0.7);
		animation->setEndValue(1.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else if ((watched == minerButton || watched == beltButton || watched == cutterButton) && event->type() == QEvent::Leave)
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