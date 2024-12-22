#ifndef CUTTER_H
#define CUTTER_H

#include <QPixmap>
#include "parameters.h"
#include "device.h"

class Cutter : public Device
{
	Q_OBJECT

public:
	explicit Cutter(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event);

private:
	QVector<QPixmap> pixmap;
};

#endif