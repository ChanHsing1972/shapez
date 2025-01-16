// Created by ChenXin.
// 定义切割机类，继承自 Device 类，
// 实现了切割机的绘制和鼠标点击事件。

#ifndef CUTTER_H
#define CUTTER_H

#include "device.h"

class Cutter : public Device
{
	Q_OBJECT

public:
	explicit Cutter(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	QVector<QPixmap> pixmap;
	QVector<QVector<Device*>>* devices;
	int inputDirection; // 输入矿物的方向
	int outputDirection; // 输出矿物的方向
};

#endif