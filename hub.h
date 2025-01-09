// hub.h
#ifndef HUB_H
#define HUB_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include "parameters.h"
#include "device.h"
#include "mineral.h"

class Hub : public Device
{
	Q_OBJECT

public:
	explicit Hub(QWidget* parent = nullptr);
	void setPosition(int x, int y);
	void increaseCount(); // 增加计数
	void reduceMineralCount(int count); // 减少计数
	int getMineralCount(); // 获取矿物数量

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void updateIcon();
	int mineralCount;
};

#endif // HUB_H