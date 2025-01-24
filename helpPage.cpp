// Created by ChenXin.
// 帮助页面的类的实现

#include "helpPage.h"

HelpPage::HelpPage(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("帮助");
	setFixedSize(900, 1250);

	QVBoxLayout* layout = new QVBoxLayout(this);
	QLabel* helpTextLabel = new QLabel(this);

	helpTextLabel->setText(
		"<style>"
		"p { line-height: 1.2; }"
		"a { color: #1E90FF; text-decoration: none; }"
		"</style>"
		"<h2>关于</h2>"
		"<p>此程序是一个模仿<a href='http://shapez.io/'>异形工厂</a>（Shapez）的游戏。<br>\
		    您可在此处找到游戏方法及注意事项。</p>"
		"<h3>说明</h3>"
		"<p>此程序由<a href='https://chenxinchen.us.kg/'>尘心</a>独立开发制作，图像素材来自<a href='https://github.com/jiangqianyu/MyShapez'>此仓库</a>。</p>"
		"<h3>游戏方法</h3>"
		"<p>在地图上放置传送带、开采机、切割机、垃圾桶四种设施，运送或制造<br>\
			  不同矿物至交付中心，获得点数。在商店，用点数为设施更新换代。"
		"<ul>"
		"		 <li> 左键单击/拖动：放置设施"
		"    <li> 右键单击/拖动：删除设施"
		"    <li> 1/2/3/4：选择设施"
		"    <li> R：旋转设施"
		"    <li> Esc：退出游戏"
		"</ul>"
		"四种设施的功能如下。"
		"<ul>"
		"		 <li> 传送带：运送物品。"
		"		 <li> 开采器：放置在矿地，开采矿物。"
		"		 <li> 切割机：将物品纵向切开并分别输出。如果只需要其中一半的物品，<br>\
										 请使用垃圾桶清除另一半物品，否则切割机将停止工作。"
		"		 <li> 垃圾桶：从四个方向输入矿物，并永久清除它们。"
		"</ul></p>"
		"<h3>注意事项</h3>"
		"<p>放置传送带时，为确保传送带能够正确实现自动转弯，请匀速拖动鼠标，<br>\
			  <strong>使其始终处于格子中线附近</strong>。</p>"
	);

	// 设置字体和边距
	int fontYaHei = QFontDatabase::addApplicationFont("./微软雅黑.ttc");
	QString fontYaHeiFamily = QFontDatabase::applicationFontFamilies(fontYaHei).at(0);
	QFont customFontYaHei(fontYaHeiFamily, 12);

	helpTextLabel->setFont(customFontYaHei); // 设置字体
	helpTextLabel->setContentsMargins(0, 0, 0, 0); // 设置边距
	helpTextLabel->setOpenExternalLinks(true); // 允许打开超链接

	layout->addWidget(helpTextLabel);
	layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

	// 添加关闭按钮
	QPushButton* closeButton = new QPushButton("关闭", this);

	int fontPingFang = QFontDatabase::addApplicationFont("./PingFang-Regular.ttf");
	QString fontPingFangFamily = QFontDatabase::applicationFontFamilies(fontPingFang).at(0);
	QFont customFontPingFang(fontPingFangFamily);

	closeButton->setFont(customFontPingFang);
	closeButton->setFixedSize(100, 40);
	closeButton->setStyleSheet(
		"QPushButton {"
		"   font-size: 24px;"
		"   background-color: #1F1F1F;"
		"   color: white;"
		"   border-radius: 10px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #3A3A3A;"
		"}"
		"QPushButton:pressed {"
		"   background-color: #575757;"
		"   padding-left: 2px;"
		"   padding-top: 2px;"
		"}");
	layout->addWidget(closeButton);
	connect(closeButton, &QPushButton::clicked, this, &HelpPage::accept);

	layout->setContentsMargins(60, 60, 60, 60);
	layout->setAlignment(Qt::AlignTop);
}