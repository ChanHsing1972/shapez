// 传送带、采矿机、垃圾桶等所有工具的基类

#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include "parameters.h"

class Device : public QWidget
{
	Q_OBJECT

public:
	explicit Device(QWidget* parent = nullptr);
	virtual void paintEvent(QPaintEvent* event) = 0; // 纯虚函数
	void mousePressEvent(QMouseEvent* event) override; // 处理鼠标单击事件
	void setPosition(int x, int y); // 设置位置
	void setRotationState(int state); // 设置旋转方向
	int getX(); // 获取 X 坐标（单位：格）
	int getY(); // 获取 Y 坐标（单位：格）
	int getRotationState(); // 获取旋转状态
	int getTypeID(); // 获取设备类型

protected:
	int posX, posY; // 设备位置
	int rotationState = 0; // 旋转状态
	int typeID; // 设备类型

signals:
	void rightClicked(Device* device);
};

#endif