#include "comletePage.h"

CompletePage::CompletePage(QWidget* parent) : QWidget(parent) {
	setGeometry(0, 0, parent->width(), parent->height());
	setStyleSheet("background-color: rgb(230,230,230)");
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	int fontPingFang = QFontDatabase::addApplicationFont("./PingFang-Regular.ttf");
	QString fontPingFangFamily = QFontDatabase::applicationFontFamilies(fontPingFang).at(0);
	QFont customFontPingFang(fontPingFangFamily);

	completionLabel = new QLabel("<strong>任务完成</strong>", this);
	completionLabel->setFont(customFontPingFang);
	completionLabel->setStyleSheet("color: gray; font-size: 120px;");
	completionLabel->setAlignment(Qt::AlignCenter);
	completionLabel->setGeometry(0, 0, parent->width(), parent->height());

	QString buttonStyleSheet =
		R"(
        QPushButton {
            font-size: 28px;
            background-color: #008700;
            color: white;
            border-radius: 20px;
        }
        QPushButton:hover {
            background-color: #008700;
        }
        QPushButton:pressed {
            background-color: #297E33;
            padding-left: 3px;
            padding-top: 3px;
        }
    )";

	continueButton = new QPushButton("继续", this);
	continueButton->setFont(customFontPingFang);
	continueButton->setStyleSheet(buttonStyleSheet);
	continueButton->setGeometry(parent->width() / 2 - 50, parent->height() * 2 / 3, 100, 50);
	connect(continueButton, &QPushButton::clicked, this, &CompletePage::continueClicked);

	continueButton->setAttribute(Qt::WA_Hover, true);
	QGraphicsOpacityEffect* continueOpacityEffect = new QGraphicsOpacityEffect(continueButton);
	continueButton->setGraphicsEffect(continueOpacityEffect);
	continueButton->setProperty("opacityEffect", QVariant::fromValue(continueOpacityEffect));
	continueButton->installEventFilter(this);

	hide();
}

bool CompletePage::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == continueButton && event->type() == QEvent::Enter)
	{
		// 鼠标进入按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(0.7);
		animation->setEndValue(1.0);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else if (watched == continueButton && event->type() == QEvent::Leave)
	{
		// 鼠标离开按钮
		QGraphicsOpacityEffect* opacityEffect = watched->property("opacityEffect").value<QGraphicsOpacityEffect*>();
		QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity", this);
		animation->setDuration(200);
		animation->setStartValue(1.0);
		animation->setEndValue(0.7);
		animation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	return QWidget::eventFilter(watched, event);
}