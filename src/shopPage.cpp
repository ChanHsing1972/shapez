// Created by Chenxin.
// 商店页面类的实现。

#include "shopPage.h"

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50

ShopPage::ShopPage(QVector<QVector<Device*>>& devices, QWidget* parent)
	: QDialog(parent), devices(devices)
{
	setWindowTitle("商店");
	setFixedSize(WINDOW_WIDTH / 2.5, WINDOW_HEIGHT / 2.25);
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
            font-size: 24px;
            background-color: #008700;
            color: white;
            border-radius: 15px;
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
	QString labelStyleSheet = "font-size: 40px; color: #666870;";
	QString costLabelStyleSheet = "font-size: 24px; color: #000000;";
	QString sectionStyleSheet = "background-color: #dddddd; border-radius: 20px;";

	// 开采器布局
	QWidget* minerSection = new QWidget(this);
	minerSection->setStyleSheet(sectionStyleSheet);
	QVBoxLayout* minerSectionLayout = new QVBoxLayout(minerSection);

	QHBoxLayout* minerLayout = new QHBoxLayout();
	QLabel* minerIcon = new QLabel(this);
	minerIcon->setAlignment(Qt::AlignCenter);
	minerIcon->setPixmap(QPixmap("./assets/images/miner_button.png").scaled(90, 90, Qt::KeepAspectRatio));
	QVBoxLayout* minerInfoLayout = new QVBoxLayout();
	QLabel* minerLabel = new QLabel("<strong>开采器</strong>", this);
	minerLabel->setFont(customFontYaHei);
	minerLabel->setStyleSheet(labelStyleSheet);
	minerLevelLabel = new QLabel(QString("等级 %1").arg(minerLevel), this);
	minerLevelLabel->setFont(customFontYaHei);
	minerLevelLabel->setStyleSheet(costLabelStyleSheet);
	minerInfoLayout->addSpacing(50);
	minerInfoLayout->addWidget(minerLabel);
	minerInfoLayout->addWidget(minerLevelLabel);
	minerInfoLayout->addSpacing(50);

	QVBoxLayout* minerCostLayout = new QVBoxLayout();
	minerCostLabel = new QLabel(QString("-%1 摩拉").arg(minerCost), this);
	minerCostLabel->setFont(customFontYaHei);
	minerCostLabel->setAlignment(Qt::AlignCenter);
	minerCostLabel->setStyleSheet(costLabelStyleSheet);
	minerButton = new QPushButton("升级", this);
	minerButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	minerButton->setFont(customFontPingFang);
	minerButton->setStyleSheet(buttonStyleSheet);
	minerCostLayout->addSpacing(50);
	minerCostLayout->addWidget(minerCostLabel);
	minerCostLayout->addWidget(minerButton);
	minerCostLayout->addSpacing(50);

	minerLayout->addSpacing(60);
	minerLayout->addWidget(minerIcon);
	minerLayout->addSpacing(15);
	minerLayout->addLayout(minerInfoLayout);
	minerLayout->addStretch();
	minerLayout->addLayout(minerCostLayout);
	minerLayout->addSpacing(75);
	minerSectionLayout->addLayout(minerLayout);

	// 传送带布局
	QWidget* beltSection = new QWidget(this);
	beltSection->setStyleSheet(sectionStyleSheet);
	QVBoxLayout* beltSectionLayout = new QVBoxLayout(beltSection);

	QHBoxLayout* beltLayout = new QHBoxLayout();
	QLabel* beltIcon = new QLabel(this);
	beltIcon->setAlignment(Qt::AlignCenter);
	beltIcon->setPixmap(QPixmap("./assets/images/belt_button.png").scaled(85, 85, Qt::KeepAspectRatio));
	QVBoxLayout* beltInfoLayout = new QVBoxLayout();
	QLabel* beltLabel = new QLabel("<strong>传送带</strong>", this);
	beltLabel->setFont(customFontYaHei);
	beltLabel->setStyleSheet(labelStyleSheet);
	beltLevelLabel = new QLabel(QString("等级 %1").arg(beltLevel), this);
	beltLevelLabel->setFont(customFontYaHei);
	beltLevelLabel->setStyleSheet(costLabelStyleSheet);
	beltInfoLayout->addSpacing(50);
	beltInfoLayout->addWidget(beltLabel);
	beltInfoLayout->addWidget(beltLevelLabel);
	beltInfoLayout->addSpacing(50);

	QVBoxLayout* beltCostLayout = new QVBoxLayout();
	beltCostLabel = new QLabel(QString("-%1 摩拉").arg(beltCost), this);
	beltCostLabel->setFont(customFontYaHei);
	beltCostLabel->setAlignment(Qt::AlignCenter);
	beltCostLabel->setStyleSheet(costLabelStyleSheet);
	beltButton = new QPushButton("升级", this);
	beltButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	beltButton->setFont(customFontPingFang);
	beltButton->setStyleSheet(buttonStyleSheet);
	beltCostLayout->addSpacing(50);
	beltCostLayout->addWidget(beltCostLabel);
	beltCostLayout->addWidget(beltButton);
	beltCostLayout->addSpacing(50);

	beltLayout->addSpacing(60);
	beltLayout->addWidget(beltIcon);
	beltLayout->addSpacing(15);
	beltLayout->addLayout(beltInfoLayout);
	beltLayout->addStretch();
	beltLayout->addLayout(beltCostLayout);
	beltLayout->addSpacing(75);
	beltSectionLayout->addLayout(beltLayout);

	// 切割机布局
	QWidget* cutterSection = new QWidget(this);
	cutterSection->setStyleSheet(sectionStyleSheet);
	QVBoxLayout* cutterSectionLayout = new QVBoxLayout(cutterSection);

	QHBoxLayout* cutterLayout = new QHBoxLayout();
	QLabel* cutterIcon = new QLabel(this);
	cutterIcon->setAlignment(Qt::AlignCenter);
	cutterIcon->setPixmap(QPixmap("./assets/images/cutter_button.png").scaled(85, 85, Qt::KeepAspectRatio));
	QVBoxLayout* cutterInfoLayout = new QVBoxLayout();
	QLabel* cutterLabel = new QLabel("<strong>切割机</strong>", this);
	cutterLabel->setFont(customFontYaHei);
	cutterLabel->setStyleSheet(labelStyleSheet);
	cutterLevelLabel = new QLabel(QString("等级 %1").arg(cutterLevel), this);
	cutterLevelLabel->setFont(customFontYaHei);
	cutterLevelLabel->setStyleSheet(costLabelStyleSheet);
	cutterInfoLayout->addSpacing(50);
	cutterInfoLayout->addWidget(cutterLabel);
	cutterInfoLayout->addWidget(cutterLevelLabel);
	cutterInfoLayout->addSpacing(50);

	QVBoxLayout* cutterCostLayout = new QVBoxLayout();
	cutterCostLabel = new QLabel(QString("-%1 摩拉").arg(cutterCost), this);
	cutterCostLabel->setFont(customFontYaHei);
	cutterCostLabel->setAlignment(Qt::AlignCenter);
	cutterCostLabel->setStyleSheet(costLabelStyleSheet);
	cutterButton = new QPushButton("升级", this);
	cutterButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	cutterButton->setFont(customFontPingFang);
	cutterButton->setStyleSheet(buttonStyleSheet);
	cutterCostLayout->addSpacing(50);
	cutterCostLayout->addWidget(cutterCostLabel);
	cutterCostLayout->addWidget(cutterButton);
	cutterCostLayout->addSpacing(50);

	cutterLayout->addSpacing(60);
	cutterLayout->addWidget(cutterIcon);
	cutterLayout->addSpacing(15);
	cutterLayout->addLayout(cutterInfoLayout);
	cutterLayout->addStretch();
	cutterLayout->addLayout(cutterCostLayout);
	cutterLayout->addSpacing(75);
	cutterSectionLayout->addLayout(cutterLayout);

	// 将各个布局添加到主布局中
	mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Fixed, QSizePolicy::Minimum));
	mainLayout->addWidget(minerSection);
	mainLayout->addSpacerItem(new QSpacerItem(0, 3, QSizePolicy::Fixed, QSizePolicy::Minimum));
	mainLayout->addWidget(beltSection);
	mainLayout->addSpacerItem(new QSpacerItem(0, 3, QSizePolicy::Fixed, QSizePolicy::Minimum));
	mainLayout->addWidget(cutterSection);
	mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Fixed, QSizePolicy::Minimum));

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
		if (hub->getMineralCount() < minerCost)
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
	minerCostLabel->setText(QString("-%1 摩拉").arg(minerCost));
	minerCostLabel->setAlignment(Qt::AlignCenter);

	QMessageBox::information(this, "成功", "已修改开采器速度。");
}

void ShopPage::applyBeltSpeed()
{
	if (Hub* hub = dynamic_cast<Hub*>((devices)[(WINDOW_WIDTH / GRID_SIZE - 2) / 2][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2]))
	{
		if (hub->getMineralCount() < beltCost)
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
	beltCostLabel->setText(QString("-%1 摩拉").arg(beltCost));
	beltCostLabel->setAlignment(Qt::AlignCenter);

	QMessageBox::information(this, "成功", "已修改传送带速度。");
}

void ShopPage::applyCutterSpeed()
{
	if (Hub* hub = dynamic_cast<Hub*>((devices)[(WINDOW_WIDTH / GRID_SIZE - 2) / 2][(WINDOW_HEIGHT / GRID_SIZE - 2) / 2]))
	{
		if (hub->getMineralCount() < cutterCost)
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
	cutterCostLabel->setText(QString("-%1 摩拉").arg(cutterCost));
	cutterCostLabel->setAlignment(Qt::AlignCenter);

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