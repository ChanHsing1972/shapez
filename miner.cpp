// Created by ChenXin.
// 开采器类的实现。

#include "Miner.h"

Miner::Miner(QVector<QVector<Device*>>* devices, QWidget* parent)
	: Device(parent), devices(devices), pixmap(4), mineralType(0)
{
	setFixedSize(GRID_SIZE, GRID_SIZE);
	typeID = 1;

	pixmap[_W].load("./assets/images/miner_W.png");
	pixmap[_D].load("./assets/images/miner_D.png");
	pixmap[_S].load("./assets/images/miner_S.png");
	pixmap[_A].load("./assets/images/miner_A.png");

	generateTimer = new QTimer(this);                                        // 创建定时器
	connect(generateTimer, &QTimer::timeout, this, &Miner::generateMineral); // 定时器超时信号与生成矿物槽函数连接
	generateTimer->start(MINER_SPEED);                                       // 每 MINER_SPEED 秒生成一个矿物
}

void Miner::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	raise();
	painter.drawPixmap(rect(), pixmap[rotationState].scaled(size(), Qt::KeepAspectRatio));
}

void Miner::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}

void Miner::setMineralType(int type)
{
	mineralType = type;
}

void Miner::generateMineral() // 生成矿物
{
	generateTimer->stop();                                                   // 停止定时生成矿物
	Mineral* newMineral = new Mineral(devices, mineralType, parentWidget()); // 创建新矿物
	newMineral->setPosition(posX, posY);                                     // 矿物位置
	newMineral->setDirection(rotationState);                                 // 矿物移动方向
	GameMap::mineralList.append(newMineral);                                 // 将新生成的矿物添加到全局列表
	emit newMineralGenerated(newMineral);                                    // 发出生成新矿物的信号
	generateTimer->start(MINER_SPEED);                                       // 生成下一个矿物
}
