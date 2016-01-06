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
 *	@file LogWidget.cpp
 */
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
