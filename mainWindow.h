#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
//#include "ui_mainWindow.h"
#include "parameters.h"
#include "gameMap.h"
#include "belt.h"
#include "miner.h"
#include "cutter.h"
#include "welcomePage.h"

class mainWindow : public QWidget
{
	Q_OBJECT

public:
	mainWindow(QWidget* parent = nullptr);
	~mainWindow();

private:
	//Ui::WidgetClass ui;
};

#endif
