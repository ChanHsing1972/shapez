// Created by Chenxin.
// 垃圾桶类，继承自Device类，
// 用于显示垃圾桶图标，实现拖拽删除功能。

#ifndef TRASH_H
#define TRASH_H

#include <QPixmap>
#include "parameters.h"
#include "device.h"

class Trash : public Device
{
	Q_OBJECT

public:
	explicit Trash(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event);

private:
	QVector<QPixmap> pixmap;
};

#endif