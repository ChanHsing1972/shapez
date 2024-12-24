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
	painter.setPen(Qt::darkGray);
	painter.setFont(QFont("Impact", 18));
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