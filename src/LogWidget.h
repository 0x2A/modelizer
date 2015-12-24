#pragma once

#include <QtWidgets/QPlainTextEdit>
#include <QFile>

class LogWidget : public QPlainTextEdit
{
	Q_OBJECT

public:
	LogWidget(QWidget *parent = 0);
	void AppendMessage(const QString& text);
	void AppendWarning(const QString& text);
	void AppendError(const QString& text);

private:
};