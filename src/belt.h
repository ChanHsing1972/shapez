// Created by ChenXin.
// 传送带类，继承自 Device 类，
// 用于显示传送带的图像和处理传送带的点击事件。

#ifndef BELT_H
#define BELT_H

#include <QPixmap>
#include <QPainter>
#include "mineral.h"
#include "parameters.h"
#include "device.h"

class Belt : public Device
{
	Q_OBJECT

public:
	explicit Belt(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event);

private:
	QVector<QPixmap> pixmap;
};

#endif