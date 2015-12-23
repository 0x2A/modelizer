#ifndef MODELIZER_H
#define MODELIZER_H

#include <QtWidgets/QMainWindow>
#include "ui_modelizer.h"

class modelizer : public QMainWindow
{
	Q_OBJECT

public:
	modelizer(QWidget *parent = 0);
	~modelizer();

private:
	Ui::modelizerClass ui;
};

#endif // MODELIZER_H
