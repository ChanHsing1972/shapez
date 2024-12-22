#include "Miner.h"
#include <QPainter>

Miner::Miner(QVector<QVector<Device*>>* devices, QWidget* parent)
	: Device(parent), devices(devices), pixmap(4)

{
	setFixedSize(GRID_SIZE, GRID_SIZE);

	pixmap[_W].load("./assets/images/miner_W.png");
	pixmap[_D].load("./assets/images/miner_D.png");
	pixmap[_S].load("./assets/images/miner_S.png");
	pixmap[_A].load("./assets/images/miner_A.png");

	generateTimer = new QTimer(this);
	connect(generateTimer, &QTimer::timeout, this, &Miner::generateMineral);
	generateTimer->start(2000); // 每 2 秒生成一个矿物
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

void Miner::generateMineral() // 生成矿物
{
	Mineral* newMineral = new Mineral(devices, parentWidget());
	newMineral->setPosition(posX, posY);
	newMineral->setDirection(rotationState);
	GameMap::mineralList.append(newMineral); // 将新生成的矿物添加到全局列表
	emit newMineralGenerated(newMineral);
	qDebug() << "Mineral generated!";
}
