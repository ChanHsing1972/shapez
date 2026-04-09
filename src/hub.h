// Created by ChenXin.
// 交付中心类，用于显示中心的矿物数量，
// 以及增加和减少矿物数量。

#ifndef HUB_H
#define HUB_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include "parameters.h"
#include "mineral.h"
#include "device.h"

class GameMap;

class Hub : public Device
{
	Q_OBJECT

public:
	explicit Hub(QWidget* parent = nullptr);
	void setPosition(int x, int y);           // 设置交付中心位置
	void increaseMineralCount(int countType); // 增加矿物数量
	void reduceMineralCount(int count);       // 减少矿物数量
	int getMineralCount();                    // 获取矿物数量
	void setMineralCount(int count);          // 设置矿物数量

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	GameMap* gameMap;              // GameMap 引用
	QPixmap pixmap;                // 交付中心图标
	QVector<int> mineralTypeCount; // 记录各种类型矿物的数量
	int mineralCount;              // 矿物总数量
	void updateIcon();             // 更新数量图标
};

#endif