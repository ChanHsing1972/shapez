// cutter.h
#ifndef CUTTER_H
#define CUTTER_H

#include "device.h"

class Cutter : public Device {
	Q_OBJECT

public:
	explicit Cutter(QVector<QVector<Device*>>* devices, QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	QVector<QPixmap> pixmap;
	QVector<QVector<Device*>>* devices;
	int inputDirection;
	int outputDirection;
};

#endif // CUTTER_H