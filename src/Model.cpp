#include "Model.h"
#include "modelizer.h"
#include <QFile>
#include <QFileInfo>

Model* Model::Load(const QString path, unsigned int flags)
{
	if (!QFile::exists(path))
	{
		modelizer::Log->AppendError("Couldn't open file: " + path);
		return nullptr;
	}

	qApp->processEvents();

	Model *model = new Model;
	model->m_Scene = model->m_Importer.ReadFile(path.toStdString(), flags);
	// If the import failed, report it
	if (!model->m_Scene)
	{
		modelizer::Log->AppendError(model->m_Importer.GetErrorString());
		delete model;
		return nullptr;
	}

	QFileInfo finfo(path);
	model->m_BasePath = finfo.absolutePath();

	modelizer::Log->AppendMessage("begin loading textures");

	qApp->processEvents();
	if (!model->LoadGLTextures())
	{
		return model;
	}

	modelizer::Log->AppendMessage("Scene import succeeded.");
	return model;
}

bool Model::LoadGLTextures()
{
	if (m_Scene->HasTextures())
	{
		modelizer::Log->AppendWarning("No textures embedded in this file.");
		return false;
	}

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m = 0; m < m_Scene->mNumMaterials; m++)
	{
		qApp->processEvents();
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;
		aiString path;	// filename
		while (texFound == AI_SUCCESS)
		{
			qApp->processEvents();
			texFound = m_Scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			m_TextureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	bool err = false;
	for (auto &itr : m_TextureIdMap)
	{
		qApp->processEvents();
		QString filename = QString::fromStdString(itr.first);
		if (filename.isEmpty()) continue;

		filename = m_BasePath + "/" + filename;

		modelizer::Log->AppendMessage("Loading texture '" + filename + "'");
		QImage image;
		if (!image.load(filename))
		{
			modelizer::Log->AppendError("Failed to load texture '" + filename + "'");
			itr.second = nullptr;
			err = true;
			continue;
		}
		itr.second = new QOpenGLTexture(image);

		itr.second->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	}
	return !err;
}

Model::~Model()
{
	for (auto &itr : m_TextureIdMap)
	{
		delete itr.second;
	}
	m_TextureIdMap.clear();
}

Model::Model()
{

}
