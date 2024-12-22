// mineral.h
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
	explicit Mineral(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void setPosition(int x, int y);
	void setDirection(int direction);
	int getDirection();
	void startMoving();
	void stopMoving();
	int getX() const;
	int getY() const;

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void moving();

private:
	int pixelX, pixelY;
	int minerDirection;
	int direction;
	QPixmap mineralImage;
	QTimer* moveTimer;
	QTimer* checkTimer;
	QVector<QVector<Device*>>* devices;
	void checkForBelt();
	bool isMineralAhead();
};

#endif // MINERAL_H
