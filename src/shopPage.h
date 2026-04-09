// Created by Chenxin.
// 商店页面类，实现商店页面的功能，包括购买升级设备等。

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
	QLabel* minerCostLabel;
	QLabel* beltCostLabel;
	QLabel* cutterCostLabel;
	QPushButton* minerButton;
	QPushButton* beltButton;
	QPushButton* cutterButton;
};

#endif // SHOPPAGE_H