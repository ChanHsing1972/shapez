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
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

signals:
	void positionChanged(int x, int y);

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void moving();

private:
	int pixelX, pixelY;
	int minerDirection;
	int direction;
	bool directionInitiated = false;
	bool isDeleting = false;
	QPixmap mineralImage;
	QTimer* moveTimer;
	QVector<QVector<Device*>>* devices;
};

#endif // MINERAL_H
