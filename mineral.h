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
	explicit Mineral(QVector<QVector<Device*>>* devices, int mineralType, QWidget* parent = nullptr);
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
	int mineralType;
	QPixmap mineralImage;
	QTimer* moveTimer;
	QTimer* checkTimer;
	QVector<QVector<Device*>>* devices;
	void checkForBelt();
	bool isMineralAhead();
	bool checkIfAtHub();
	int getNextGridX(int gridX);
	int getNextGridY(int gridY);

signals:
	void deliveredToHub();
};

#endif // MINERAL_H
