#ifndef MINER_H
#define MINER_H

#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include "parameters.h"
#include "device.h"
#include "mineral.h"

class Mineral;

class Miner : public Device
{
	Q_OBJECT

public:
	explicit Miner(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void setMineralType(int type);

signals:
	void newMineralGenerated(Mineral* mineral);

private slots:
	void generateMineral();

private:
	int mineralType;
	QVector<QPixmap> pixmap;
	QTimer* generateTimer;
	QVector<QVector<Device*>>* devices;
};

#endif // MINER_H