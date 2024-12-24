// cutter.h
#ifndef CUTTER_H
#define CUTTER_H

#include "device.h"
#include <QQueue>
#include <QTimer>
#include "gameMap.h"

class Mineral;
class Cutter : public Device {
	Q_OBJECT

public:
	explicit Cutter(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	int getInputDirection();
	int getOutputDirection();
	void receiveMineral(Mineral* mineral);

signals:
	void newMineralGenerated(Mineral* mineral);

private slots:
	void processMineral();

private:
	QVector<QPixmap> pixmap;
	QVector<QVector<Device*>>* devices;
	int inputDirection;
	int outputDirection;
	QTimer* processTimer;
	QQueue<Mineral*> mineralQueue;
};

#endif // CUTTER_H