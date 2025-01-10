#pragma execution_character_set("utf-8")
#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEvent>
#include <QMediaplayer>
#include <QMediaPlaylist>
#include "parameters.h"

class WelcomePage : public QWidget {
	Q_OBJECT

public:
	explicit WelcomePage(QWidget* parent = nullptr);

signals:
	void startGame(); // 开始游戏信号
	void loadGame(const QString& saveFileName); // 继续游戏信号

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	QPushButton* startButton;
	QPushButton* loadButton;
	QPushButton* exitButton;
	QMediaPlayer* bgmPlayer;
	QMediaPlaylist* bgmPlaylist;

private slots:
	void onStartClicked(); // 开始按钮点击槽
	void onLoadClicked(); // 继续按钮点击槽
	void onExitClicked(); // 退出按钮点击槽
};

#endif