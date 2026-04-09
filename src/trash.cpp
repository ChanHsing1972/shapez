// Created by Chenxin.
// 垃圾桶类的实现。

#include "Trash.h"

Trash::Trash(QWidget* parent) : Device(parent), pixmap(4)
{
	setFixedSize(GRID_SIZE, GRID_SIZE);
	typeID = 3;

	pixmap[_W].load("./assets/images/trash_W.png");
	pixmap[_D].load("./assets/images/trash_D.png");
	pixmap[_S].load("./assets/images/trash_S.png");
	pixmap[_A].load("./assets/images/trash_A.png");
}

void Trash::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	raise();
	QPainter painter(this);
	painter.drawPixmap(rect(), pixmap[rotationState].scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio));
}

void Trash::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}