/*
	Modelizer - Model file converter
	Copyright (C) 2015 Frank K�hnke

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
 *	@file LogWidget.h
 */
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