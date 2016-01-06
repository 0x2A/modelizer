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
 *	@file ModelViewWidget.h
 */
#pragma once

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "OrbitCamera.h"


enum class RenderMode
{
	blinn,
	unlit,
	wireframe
};

class ModelViewWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit ModelViewWidget(QWidget *parent = 0);
	~ModelViewWidget();

	void SetRenderMode(RenderMode mode);
	
protected:

	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent *event);

	void keyPressEvent(QKeyEvent * event);

signals:

public slots :

private:
	void SetupShaders();
	void SetupGridMesh();

	QOpenGLBuffer m_GridVbo;
	QOpenGLVertexArrayObject m_GridVao;
	int m_GridVertexCount;

	OrbitCamera *m_Camera;
	QMatrix4x4 m_GridModelMatrix;

	QOpenGLShaderProgram m_GridShader;
	QOpenGLShaderProgram m_UnlitShader;
	QOpenGLShaderProgram m_BlinnShader;

	RenderMode m_RenderMode;

	QPoint m_LastMousePos;
};