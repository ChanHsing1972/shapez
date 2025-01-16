// Created by ChenXin.
// 显示帮助页面的类

#pragma execution_character_set("utf-8")
#ifndef HELPPAGE_H
#define HELPPAGE_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QFont>
#include <QDesktopServices>
#include <QSpacerItem>
#include <QFontDatabase>

class HelpPage : public QDialog
{
	Q_OBJECT

public:
	HelpPage(QWidget* parent = nullptr);
};

#endif 