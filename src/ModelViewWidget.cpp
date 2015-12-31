
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

	m_RenderMode = RenderMode::blinn;
}

ModelViewWidget::~ModelViewWidget()
{
	delete m_Camera;
	m_Camera = nullptr;
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

	m_Camera = new OrbitCamera(1.77f, 41.112f, QVector3D(0, 0, 0), QVector3D(0, 1, 0), 0.1f, 1000.0f);
	m_Camera->SetPitch(45.0f);
	m_Camera->SetYaw(-19.45f);
	m_GridModelMatrix.translate(QVector3D(-10, 0, -10));
}

void ModelViewWidget::paintGL()
{

	m_GridShader.bind();
	m_GridVao.bind();

	m_GridShader.setUniformValue("MVPMatrix", m_Camera->ViewProjectionmatrix()*m_GridModelMatrix);
	glDrawArrays(GL_LINES, 0, m_GridVertexCount);
	m_GridVao.release();

	if (modelizer::m_Model)
	{
		QOpenGLShaderProgram *shader;
		switch (m_RenderMode)
		{
		case RenderMode::wireframe:
			shader = &m_UnlitShader;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case RenderMode::unlit:
			shader = &m_UnlitShader;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
		case RenderMode::blinn:
			shader = &m_BlinnShader;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
			
		shader->bind();
		shader->setUniformValue("VPMatrix", m_Camera->ViewProjectionmatrix());
		shader->setUniformValue("ViewMatrix", m_Camera->ViewMatrix());
		modelizer::m_Model->Render(shader);
	}
}

void ModelViewWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_Camera->SetAspectRatio((float)width / (float)height);
}

void ModelViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint mPos = event->pos();
	QPoint delta = mPos - m_LastMousePos;

	auto buttons = event->buttons();
	if (buttons & Qt::LeftButton || buttons & Qt::RightButton)
	{
		m_Camera->SetPitch(m_Camera->Pitch() - delta.x()*0.35f);
		m_Camera->SetYaw(m_Camera->Yaw() - delta.y()*0.35f);
	}
	else if (buttons & Qt::MiddleButton)
	{
		QVector3D v = m_Camera->Right() * -delta.x() * 0.004f * m_Camera->Zoom();
		v += m_Camera->Up() * -delta.y() * 0.004f * m_Camera->Zoom();
		m_Camera->SetLookAt(m_Camera->LookAt() + v);
	}
	m_LastMousePos = mPos;
	update();
}

void ModelViewWidget::mousePressEvent(QMouseEvent * event)
{
	m_LastMousePos = event->pos();
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


	//unlit shader
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

	//blinn shader
	if (!m_BlinnShader.addShaderFromSourceCode(QOpenGLShader::Vertex,
#include "Shaders/blinn.vs"
		))
		modelizer::Log->AppendError(m_BlinnShader.log());
	if (!m_BlinnShader.addShaderFromSourceCode(QOpenGLShader::Fragment,
#include "Shaders/blinn.fs"
		))
		modelizer::Log->AppendError(m_BlinnShader.log());

	if (!m_BlinnShader.link())
	{
		modelizer::Log->AppendError(m_BlinnShader.log());
	}
}

void ModelViewWidget::keyPressEvent(QKeyEvent * event)
{
}

void ModelViewWidget::SetRenderMode(RenderMode mode)
{
	m_RenderMode = mode;
}

void ModelViewWidget::wheelEvent(QWheelEvent *event)
{
	m_Camera->SetZoom(m_Camera->Zoom() - ((event->delta() / 240.0f)*qBound(0.05f, m_Camera->Zoom(), 2.0f)));
	QOpenGLWidget::wheelEvent(event);
	update();
}

