#ifndef MODELIZER_H
#define MODELIZER_H

#include <QtWidgets/QMainWindow>
#include <QMap>
#include "ui_modelizer.h"
#include "Model.h"

#define MODELIZER_VERSION_STR	"0.1"

class modelizer : public QMainWindow
{
	Q_OBJECT

public:
	modelizer(QWidget *parent = 0);
	~modelizer();


	static LogWidget *Log;
	static Model *m_Model;
	
private slots:

	void on_actionImport_triggered();
	void on_actionExport_triggered();
	void on_actionBlinn_triggered();
	void on_actionUnlit_triggered();
	void on_actionWireframe_triggered();
	void on_actionOpenHelpMenu_triggered();
	void on_actionAbout_triggered();
	void on_actionAboutQt_triggered();

	void onLoadModel(const QString path, unsigned int flags);
	void onExportModel(const QString path, const QString formatId, unsigned int flags);

signals:

	void BeginLoadModel(const QString path, unsigned int flags);
	void BeginSaveModel(const QString path, const QString formatId, unsigned int flags);

private:

	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *event);
	void InitToolbar();

	Ui::modelizerClass ui;
	QMap<QString, QString> m_ExportTypeMap;


};

#endif // MODELIZER_H
