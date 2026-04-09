// Created by ChenXin.
// 显示完成页面，包括完成提示和继续按钮。

#pragma execution_character_set("utf-8")
#ifndef COMPLETEPAGE_H
#define COMPLETEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QVariant>
#include <QEvent>
#include <QPropertyAnimation>
#include "parameters.h"

class CompletePage : public QWidget
{
	Q_OBJECT

public:
	explicit CompletePage(QWidget* parent = nullptr);

signals:
	void continueClicked();

private:
	QLabel* completionLabel;
	QPushButton* continueButton;
	bool eventFilter(QObject* watched, QEvent* event);
};

#endif