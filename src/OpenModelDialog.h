#pragma once


#include <QFileDialog>
#include <QtWidgets/QCheckBox>

class OpenModelDialog : public QFileDialog
{
	Q_OBJECT

public:
	OpenModelDialog(QWidget *parent = 0,
		const QString &caption = QString(),
		const QString &directory = QString(),
		const QString &filter = QString());

	void AddCheckBoxes();
	int exec();

private:

	QCheckBox* m_Cb;
};