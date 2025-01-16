// Created by ChenXin.
// 开采器类，继承自 Device 类，用于生成矿物。

#ifndef MINER_H
#define MINER_H

#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include "parameters.h"
#include "device.h"
#include "mineral.h"

class Mineral;

class Miner : public Device
{
	Q_OBJECT

public:
	explicit Miner(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void setMineralType(int type); // 设置矿物类型

signals:
	void newMineralGenerated(Mineral* mineral); // 生成新矿物的信号

private slots:
	void generateMineral(); // 生成矿物的槽函数

private:
	int mineralType; // 矿物类型
	QTimer* generateTimer; // 生成矿物的定时器
	QVector<QPixmap> pixmap;
	QVector<QVector<Device*>>* devices;
};

#endif