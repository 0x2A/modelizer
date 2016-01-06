/*
	Modelizer - Model file converter
	Copyright (C) 2015 Frank Köhnke

	This file is part of Modelizer.

	Modelizer is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License 
	as published by the Free Software Foundation; either 
	version 3 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 *	@file modelizer.h
 */
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
	QStringList m_ImportFilters;

};

#endif // MODELIZER_H
