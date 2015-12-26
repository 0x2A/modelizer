#include "Model.h"
#include "modelizer.h"
#include <QFile>

Model* Model::Load(const QString path, unsigned int flags)
{
	if (!QFile::exists(path))
	{
		modelizer::Log->AppendError("Couldn't open file: " + path);
		return nullptr;
	}

	Model *model = new Model;
	model->m_Scene = model->m_Importer.ReadFile(path.toStdString(), flags);
	// If the import failed, report it
	if (!model->m_Scene)
	{
		modelizer::Log->AppendError(model->m_Importer.GetErrorString());
		delete model;
		return nullptr;
	}

	modelizer::Log->AppendMessage("begin loading textures");
	if (!model->LoadGLTextures())
	{
		modelizer::Log->AppendError("Failed to load textures");
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
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;
		aiString path;	// filename
		while (texFound == AI_SUCCESS)
		{
			texFound = m_Scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			m_TextureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}
}

Model::~Model()
{
	
}

Model::Model()
{

}
