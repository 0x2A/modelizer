/*
	Modelizer - Model file converter
	Copyright (C) 2016 Frank Köhnke

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
 *	@file AboutWindow.cpp
 */

#include "AboutWindow.h"
#include "modelizer.h"

AboutWindow::AboutWindow(QWidget *parent /*= 0*/) : QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());

	//remove icon and question mark button
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	//set version string
	ui.lblVersion->setText("Version " + QString(MODELIZER_VERSION_STR));
}

AboutWindow::~AboutWindow()
{

}
