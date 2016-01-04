#pragma once

#include <QtWidgets/QDialog>
#include "ui_about.h"

class AboutWindow : public QDialog
{
	Q_OBJECT

public:
	AboutWindow(QWidget *parent = 0);
	~AboutWindow();

private:

	Ui::AboutDialog ui;

};