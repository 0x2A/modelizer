#include "LogWidget.h"
#include <QtWidgets/QScrollBar>

QColor colMsg(192,192,192,255);
QColor colWarning(255, 200, 97, 255);
QColor colError(255, 73, 64, 255);

void LogWidget::AppendMessage(const QString& text)
{
	QTextCharFormat fmt;
	fmt.setForeground(QBrush(colMsg));
	this->setCurrentCharFormat(fmt);
	this->appendPlainText("[info] " + text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
}

void LogWidget::AppendWarning(const QString& text)
{
	QTextCharFormat fmt;
	fmt.setForeground(QBrush(colWarning));
	this->setCurrentCharFormat(fmt);
	this->appendPlainText("[warning] " + text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
}

void LogWidget::AppendError(const QString& text)
{
	QTextCharFormat fmt;
	fmt.setForeground(QBrush(colError));
	this->setCurrentCharFormat(fmt);
	this->appendPlainText("[error] " + text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
}

LogWidget::LogWidget(QWidget *parent) : QPlainTextEdit(parent)
{
	
}
