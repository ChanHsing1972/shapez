// hub.cpp
#include "hub.h"

Hub::Hub(QWidget* parent) : Device(parent), mineralCount(0)
{
	setFixedSize(GRID_SIZE * 2 + 2, GRID_SIZE * 2 - 2); // 假设 hub 占据 2x2 的格子
}

void Hub::setPosition(int x, int y)
{
	move(x, y);
}

void Hub::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	// 在图标上绘制矿物数量
	int fontImpact = QFontDatabase::addApplicationFont("./assets/fonts/impact.ttf");
	QString fontImpactFamily = QFontDatabase::applicationFontFamilies(fontImpact).at(0);
	QFont customFontImpact(fontImpactFamily, 18);

	painter.setFont(customFontImpact);
	painter.setPen(Qt::darkGray);
	painter.drawText(rect(), Qt::AlignCenter, QString::number(mineralCount));
}

void Hub::increaseCount()
{
	mineralCount++;
	updateIcon();
}

void Hub::updateIcon()
{
	// 触发重绘事件，更新图标
	update();
}

void Hub::reduceMineralCount(int count)
{
	mineralCount -= count;
	updateIcon();
}

int Hub::getMineralCount()
{
	return mineralCount;
}