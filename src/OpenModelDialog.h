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
 *	@file OpenModelDialog.h
 */
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

	unsigned int GetFlags() { return flags; }

private:

	unsigned int flags;
	QCheckBox* m_CalcTangentSpace;
	QCheckBox* m_JoinIdenticalVertices;
	QCheckBox* m_MakeLeftHanded;
	QCheckBox* m_Triangulate;
	QCheckBox* m_GenNormals;
	QCheckBox* m_SplitLargeMeshes;
	QCheckBox* m_PreTransformVertices;
	QCheckBox* m_LimitBoneWeights;
	QCheckBox* m_ValidateDataStructure;
	QCheckBox* m_RemoveRedundantMaterials;
	QCheckBox* m_FixInfacingNormals;
	QCheckBox* m_SortByPType;
	QCheckBox* m_GenUVCoords;
	QCheckBox* m_TransformUVCoords;
	QCheckBox* m_OptimizeMeshes;
	QCheckBox* m_OptimizeGraph;
	QCheckBox* m_FlipUVs;
	QCheckBox* m_FlipWindingOrder;
	QCheckBox* m_Debone;
};