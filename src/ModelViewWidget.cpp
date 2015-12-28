
#include "modelizer.h"
#include "ModelViewWidget.h"
#include <QOpenGLFunctions>
#include "Mesh.h"

ModelViewWidget::ModelViewWidget(QWidget *parent /*= 0*/) : QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
#ifdef _DEBUG
	format.setOptions(QSurfaceFormat::DebugContext);
#endif
	QSurfaceFormat::setDefaultFormat(format);
}

void ModelViewWidget::initializeGL()
{
	makeCurrent();
	auto currFormat = context()->format();
	auto profile = currFormat.profile();
	QString msg = QString("OpenGL context version %1.%2 %3").arg(currFormat.majorVersion()).arg(currFormat.minorVersion()).arg(profile == QSurfaceFormat::CoreProfile ? "core profile" : "compatibility profile");
	modelizer::Log->AppendMessage(msg);

	auto f = context()->functions();
	f->glClearColor(0.15,0.15,0.15,1);

	f->glEnable(GL_DEPTH_TEST);
	f->glEnable(GL_CULL_FACE);
	f->glEnable(GL_BLEND);
	f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetupGridMesh();
	SetupShaders();

	m_ViewMatrix.lookAt(QVector3D(0,1,4),QVector3D(0,0,0), QVector3D(0,1,0));

	m_GridModelMatrix.translate(QVector3D(-10, 0, -10));
}

void ModelViewWidget::paintGL()
{

	m_GridShader.bind();
	m_GridVao.bind();

	m_GridShader.setUniformValue("MVPMatrix", m_ProjectionMatrix*m_ViewMatrix*m_GridModelMatrix);
	glDrawArrays(GL_LINES, 0, m_GridVertexCount);
	m_GridVao.release();

	if (modelizer::m_Model)
	{
		m_UnlitShader.bind();
		m_UnlitShader.setUniformValue("MVMatrix", m_ProjectionMatrix*m_ViewMatrix);
		modelizer::m_Model->Render(&m_UnlitShader);
	}
}

void ModelViewWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_ProjectionMatrix.setToIdentity();
	m_ProjectionMatrix.perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}

void ModelViewWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void ModelViewWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
		m_ViewMatrix.translate(QVector3D(0, 0, 0.1f));
	else if (event->button() == Qt::RightButton)
		m_ViewMatrix.translate(QVector3D(0, 0, -0.1f));
	update();
}

void ModelViewWidget::SetupGridMesh()
{
	m_GridVao.create();
	if (m_GridVao.isCreated())
		m_GridVao.bind();

	m_GridVbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_GridVbo.create();
	m_GridVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);	
	m_GridVbo.bind();

	VertexList vertices;
	FaceList faces;
	for (int y = 0; y < 41; y++)
	{
		Vertex v(QVector3D(0.0f, 0, y*0.5f), QVector3D(0, 1, 0), QVector2D(0, 0), QVector4D(1.0f, 1.0f, 1.0f, 0.5f));
		vertices.push_back(v);
		v = Vertex(QVector3D(20.0f, 0, y*0.5f), QVector3D(0, 1, 0), QVector2D(0, 0), QVector4D(1.0f, 1.0f, 1.0f, 0.5f));
		vertices.push_back(v);
	}
	for (int x = 0; x < 41; x++)
	{
		Vertex v(QVector3D(x*0.5f, 0, 0.0f), QVector3D(0, 1, 0), QVector2D(0, 0), QVector4D(1.0f, 1.0f, 1.0f, 0.5f));
		vertices.push_back(v);
		v = Vertex(QVector3D(x*0.5f, 0, 20.0f), QVector3D(0, 1, 0), QVector2D(0, 0), QVector4D(1.0f, 1.0f, 1.0f, 0.5f));
		vertices.push_back(v);
	}

	m_GridVbo.allocate(&vertices[0], sizeof(Vertex)*vertices.count());
	m_GridVertexCount = vertices.count();
	m_GridVbo.release();
}

void ModelViewWidget::SetupShaders()
{
	if(!m_GridShader.addShaderFromSourceCode(QOpenGLShader::Vertex, 
#include "Shaders/grid.vs"
		))
		modelizer::Log->AppendError(m_GridShader.log());
	if(!m_GridShader.addShaderFromSourceCode(QOpenGLShader::Fragment,
#include "Shaders/grid.fs"
		))
		modelizer::Log->AppendError(m_GridShader.log());

	if (!m_GridShader.link())
	{
		modelizer::Log->AppendError(m_GridShader.log());
	}
	m_GridShader.bind();
	m_GridVbo.bind();
	m_GridShader.enableAttributeArray(0);
	m_GridShader.enableAttributeArray(1);
	m_GridShader.enableAttributeArray(2);
	m_GridShader.enableAttributeArray(3);
	m_GridShader.setAttributeBuffer(MODEL_VERTEX_LOCATION, GL_FLOAT, 0, 3, sizeof(Vertex));
	m_GridShader.setAttributeBuffer(MODEL_NORMAL_LOCATION, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
	m_GridShader.setAttributeBuffer(MODEL_TEXCOORD_LOCATION, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(Vertex));
	m_GridShader.setAttributeBuffer(MODEL_VERTEX_COLOR_LOCATION, GL_FLOAT, sizeof(QVector3D) * 2 + sizeof(QVector2D), 4, sizeof(Vertex));



	if (!m_UnlitShader.addShaderFromSourceCode(QOpenGLShader::Vertex,
#include "Shaders/unlit.vs"
		))
		modelizer::Log->AppendError(m_UnlitShader.log());
	if (!m_UnlitShader.addShaderFromSourceCode(QOpenGLShader::Fragment,
#include "Shaders/unlit.fs"
		))
		modelizer::Log->AppendError(m_UnlitShader.log());

	if (!m_UnlitShader.link())
	{
		modelizer::Log->AppendError(m_UnlitShader.log());
	}
}

void ModelViewWidget::keyPressEvent(QKeyEvent * event)
{
}
