#pragma execution_character_set("utf-8")
#ifndef HELPPAGE_H
#define HELPPAGE_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QPushButton>

class HelpPage : public QDialog {
	Q_OBJECT

public:
	HelpPage(QWidget* parent = nullptr);
};

#endif 