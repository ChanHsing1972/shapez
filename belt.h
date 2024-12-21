#ifndef BELT_H
#define BELT_H

#include <QPixmap>
#include <QPainter>
#include "mineral.h"
#include "parameters.h"
#include "device.h"

class Belt : public Device
{
	Q_OBJECT

public:
	explicit Belt(QWidget* parent = nullptr);
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event);

private:
	QVector<QPixmap> pixmap;
};

#endif