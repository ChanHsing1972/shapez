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

class Hub : public QWidget
{
	Q_OBJECT

public:
	explicit Hub(QWidget* parent = nullptr);
	void setPosition(int x, int y);

protected:
	void paintEvent(QPaintEvent* event) override;

public slots:
	void addMinerals();

private:
	void updateIcon();
	int mineralCount;
};

#endif // HUB_H