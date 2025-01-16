// Created by ChenXin.
// 主窗口类，用于管理游戏的各个页面及其切换效果，
// 包括欢迎页面、游戏页面等。

#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QCloseEvent>
#include "parameters.h"
#include "welcomePage.h"
#include "gameMap.h"

class mainWindow : public QWidget
{
	Q_OBJECT

public:
	mainWindow(QWidget* parent = nullptr);
	~mainWindow();

protected:
	void closeEvent(QCloseEvent* event) override;	// 重写关闭时事件，用于在关闭窗口时保存游戏
	void keyPressEvent(QKeyEvent* event);					// 重写键盘按下事件，用于处理 Esc 键退出游戏

private:
	QStackedWidget* stackedWidget; // 管理页面的堆栈窗口
	WelcomePage* welcomePage;			 // 欢迎页面
	GameMap* gameMap;							 // 游戏页面
};

#endif
