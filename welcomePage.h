#pragma execution_character_set("utf-8")
#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include "parameters.h"

class WelcomePage : public QWidget {
	Q_OBJECT

public:
	explicit WelcomePage(QWidget* parent = nullptr);

signals:
	void startGame(); // 开始游戏信号

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	QPushButton* startButton;
	QPushButton* exitButton;

private slots:
	void onStartClicked(); // 开始按钮点击槽
	void onExitClicked(); // 退出按钮点击槽


};

#endif