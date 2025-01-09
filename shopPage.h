// shopPage.h
#pragma execution_character_set("utf-8")
#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QSpacerItem>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEvent>
#include "parameters.h"
#include "gameMap.h"
#include "hub.h"

class ShopPage : public QDialog
{
	Q_OBJECT

public:
	explicit ShopPage(QVector<QVector<Device*>>& devices, QWidget* parent = nullptr);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
	void applyBeltSpeed();
	void applyMinerSpeed();
	void applyCutterSpeed();

private:
	QVector<QVector<Device*>>& devices;
	QLabel* minerLevelLabel;
	QLabel* beltLevelLabel;
	QLabel* cutterLevelLabel;
	QPushButton* minerButton;
	QPushButton* beltButton;
	QPushButton* cutterButton;
};

#endif // SHOPPAGE_H