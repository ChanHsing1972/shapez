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
	// 重写关闭时事件，用于在关闭窗口时保存游戏
	void closeEvent(QCloseEvent* event) override;
	// 重写键盘按下事件，用于处理 Esc 键退出游戏
	void keyPressEvent(QKeyEvent* event);

private:
	QStackedWidget* stackedWidget;
	WelcomePage* welcomePage;
	GameMap* gameMap;
};

#endif
