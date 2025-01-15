// hub.cpp
#include "hub.h"

Hub::Hub(QWidget* parent) : Device(parent), mineralCount(0), mineralTypeCount(4, 0)
{
	setFixedSize(GRID_SIZE * 2 + 2, GRID_SIZE * 2 - 2); // 假设 hub 占据 2x2 的格子
	typeID = 4;

	pixmap.load("./assets/images/hub_small.png");

	gameMap = qobject_cast<GameMap*>(parent); // 初始化 GameMap 引用
}

void Hub::setPosition(int x, int y)
{
	move(x, y);
}

void Hub::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	raise();
	painter.drawPixmap(posX, posY, pixmap.scaled(GRID_SIZE * 2, GRID_SIZE * 2, Qt::KeepAspectRatio));

	// 在图标上绘制矿物数量
	int fontImpact = QFontDatabase::addApplicationFont("./impact.ttf");
	QString fontImpactFamily = QFontDatabase::applicationFontFamilies(fontImpact).at(0);
	QFont customFontImpact(fontImpactFamily, 18);

	painter.setFont(customFontImpact);
	painter.setPen(Qt::darkGray);
	painter.drawText(rect(), Qt::AlignCenter, QString::number(mineralCount));
}

void Hub::increaseMineralCount(int mineralType)
{
	mineralCount++;
	mineralTypeCount[mineralType]++;
	if (gameMap)
	{
		gameMap->updateTaskProgress(mineralTypeCount); // 更新任务进度
	}
	updateIcon();
}

void Hub::updateIcon()
{
	// 触发重绘事件，更新图标
	update();
}

void Hub::reduceMineralCount(int count)
{
	mineralCount -= count;
	updateIcon();
}

int Hub::getMineralCount()
{
	return mineralCount;
}

void Hub::setMineralCount(int count)
{
	mineralCount = count;
	updateIcon();
}