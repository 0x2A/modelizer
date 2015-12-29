#pragma once

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


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

	void SetRenderMode(RenderMode mode);
	
protected:

	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent * event);

	void keyPressEvent(QKeyEvent * event);

signals:

public slots :

private:
	void SetupShaders();
	void SetupGridMesh();

	QOpenGLBuffer m_GridVbo;
	QOpenGLVertexArrayObject m_GridVao;
	int m_GridVertexCount;

	QMatrix4x4 m_ProjectionMatrix;
	QMatrix4x4 m_ViewMatrix;
	QMatrix4x4 m_GridModelMatrix;

	QOpenGLShaderProgram m_GridShader;
	QOpenGLShaderProgram m_UnlitShader;
	QOpenGLShaderProgram m_BlinnShader;

	RenderMode m_RenderMode;
};