// Created by ChenXin.
// 传送带类的实现

#include "belt.h"

Belt::Belt(QWidget* parent) : Device(parent), pixmap(12)
{
	setFixedSize(GRID_SIZE, GRID_SIZE);
	typeID = 0;

	pixmap[_W].load("./assets/images/belt_W.png");
	pixmap[_D].load("./assets/images/belt_D.png");
	pixmap[_S].load("./assets/images/belt_S.png");
	pixmap[_A].load("./assets/images/belt_A.png");

	pixmap[_W_D].load("./assets/images/belt_W_D.png");
	pixmap[_D_S].load("./assets/images/belt_D_S.png");
	pixmap[_S_A].load("./assets/images/belt_S_A.png");
	pixmap[_A_W].load("./assets/images/belt_A_W.png");

	pixmap[_W_A].load("./assets/images/belt_W_A.png");
	pixmap[_A_S].load("./assets/images/belt_A_S.png");
	pixmap[_S_D].load("./assets/images/belt_S_D.png");
	pixmap[_D_W].load("./assets/images/belt_D_W.png");
}

void Belt::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	lower(); // 使传送带位于最底层
	QPainter painter(this);
	painter.drawPixmap(rect(), pixmap[rotationState].scaled(size(), Qt::KeepAspectRatio));
}

void Belt::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}