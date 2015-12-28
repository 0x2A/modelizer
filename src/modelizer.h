#ifndef MODELIZER_H
#define MODELIZER_H

#include <QtWidgets/QMainWindow>
#include "ui_modelizer.h"
#include "Model.h"

class modelizer : public QMainWindow
{
	Q_OBJECT

public:
	modelizer(QWidget *parent = 0);
	~modelizer();


	static LogWidget *Log;

	
private slots:

	void on_actionImport_triggered();

	void onLoadModel(const QString path, unsigned int flags);

signals:

	void BeginLoadModel(const QString path, unsigned int flags);

private:

	void InitToolbar();

	Ui::modelizerClass ui;
	Model *m_Model;

};

#endif // MODELIZER_H
