#include "cutter.h"

Cutter::Cutter(QVector<QVector<Device*>>* devices, QWidget* parent)
	: Device(parent), devices(devices), pixmap(4)
{
	pixmap[_W].load("./assets/images/cutter_W.png");
	pixmap[_D].load("./assets/images/cutter_D.png");
	pixmap[_S].load("./assets/images/cutter_S.png");
	pixmap[_A].load("./assets/images/cutter_A.png");

	// 初始化处理矿物的定时器
	processTimer = new QTimer(this);
	connect(processTimer, &QTimer::timeout, this, &Cutter::processMineral);
	processTimer->start(1000); // 每秒处理一个矿物
}

void Cutter::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);

	if (rotationState == _W || rotationState == _S)
	{
		setFixedSize(GRID_SIZE * 2, GRID_SIZE);
		painter.drawPixmap(0, 0, pixmap[rotationState].scaled(GRID_SIZE * 2, GRID_SIZE, Qt::KeepAspectRatio));
	}
	else if (rotationState == _D || rotationState == _A)
	{
		setFixedSize(GRID_SIZE, GRID_SIZE * 2);
		painter.drawPixmap(0, 0, pixmap[rotationState].scaled(GRID_SIZE, GRID_SIZE * 2, Qt::KeepAspectRatio));
	}
}

void Cutter::mousePressEvent(QMouseEvent* event)
{
	Device::mousePressEvent(event); // 调用基类处理
}

int Cutter::getInputDirection()
{
	// 根据旋转状态确定输入方向
	switch (rotationState)
	{
	case _W:
		return _S;
	case _D:
		return _A;
	case _S:
		return _W;
	case _A:
		return _D;
	default:
		return _S;
	}
}

int Cutter::getOutputDirection()
{
	// 输出方向与切割机的旋转状态一致
	return rotationState;
}

void Cutter::receiveMineral(Mineral* mineral)
{
	// 接收矿物并加入队列
	mineralQueue.enqueue(mineral);
	mineral->hide();
}

void Cutter::processMineral()
{
	if (!mineralQueue.isEmpty())
	{
		// 处理矿物，生成新的矿物
		Mineral* mineral = mineralQueue.dequeue();
		mineral->deleteLater();

		// 创建切割后的新矿物
		Mineral* newMineral = new Mineral(devices, CYCLE_MINE_L, parentWidget());
		int offsetX = 0;
		int offsetY = 0;

		// 根据输出方向设置新矿物的位置
		switch (getOutputDirection())
		{
		case _W:
			offsetY = -GRID_SIZE;
			break;
		case _D:
			offsetX = GRID_SIZE;
			break;
		case _S:
			offsetY = GRID_SIZE;
			break;
		case _A:
			offsetX = -GRID_SIZE;
			break;
		}

		newMineral->setPosition(posX + offsetX, posY + offsetY);
		newMineral->setDirection(getOutputDirection());
		newMineral->show();

		// 将新矿物加入全局矿物列表
		GameMap::mineralList.append(newMineral);
		emit newMineralGenerated(newMineral);
	}
}