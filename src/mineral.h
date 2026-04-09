// Created by ChenXin.
// 矿物类，处理地图上的矿物逻辑，
// 包括矿物的移动、碰撞检测、切割等。

#ifndef MINERAL_H
#define MINERAL_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include "parameters.h"
#include "gameMap.h"

class Device;

class Mineral : public QWidget
{
	Q_OBJECT

public:
	explicit Mineral(QVector<QVector<Device*>>* devices, int mineralType, QWidget* parent = nullptr);
	void setPosition(int x, int y);   // 设置矿物位置
	void setDirection(int direction); //	设置矿物方向
	int getDirection() const;         // 获取矿物方向
	int getX() const;                 // 获取矿物 X 坐标
	int getY() const;                 //	获取矿物 Y 坐标
	void startMoving();               // 触发定时器，开始移动矿物
	void stopMoving();                // 停止定时器，停止移动矿物
	int mineralType;                  // 矿物类型

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void moving();

private:
	int pixelX, pixelY;                     // 矿物像素坐标
	int minerDirection;                     // 开采器方向
	int direction;                          // 矿物移动方向
	QPixmap mineralImage;                   // 矿物图片
	QTimer* moveTimer;                      // 定时器，用于移动矿物
	QTimer* checkTimer;                     // 定时器，用于检查矿物前方是否有传送带
	QVector<QVector<Device*>>* devices;     // 设备列表
	void checkForBelt();                    // 循环检查矿物前方是否有新的传送带，如果有，则继续移动
	bool isMineralAhead();                  // 检查前方是否有矿物
	bool isBeltAhead();                     // 检查前方是否有传送带
	bool checkIfAtHub();                    // 检查是否到达交付中心
	bool checkIfAtCutter();                 // 检查是否到达切割机
	bool checkIfAtTrash();                  // 检查是否到达垃圾桶
	bool isMineralAt(int gridX, int gridY); // 检查指定位置是否有矿物
	int getNextGridX(int gridX);            // 获取下一个格子的 X 坐标
	int getNextGridY(int gridY);            // 获取下一个格子的 Y 坐标

signals:
	void deliveredToHub();                      // 传送到交付中心的信号
	void newMineralGenerated(Mineral* mineral); // 生成新矿物的信号
};

#endif