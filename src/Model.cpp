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

		QFileInfo finfo(filename);

		if (finfo.isRelative())
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

void Model::Render(QOpenGLShaderProgram* shader)
{
	shader->bind();
	RecursiveRender(shader, m_ModelMatrix, m_Scene, m_Scene->mRootNode);
}

static void Color4f(const aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void Model::RecursiveRender(QOpenGLShaderProgram* shader, QMatrix4x4 inpMat, const struct aiScene *sc, const struct aiNode *nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	QMatrix4x4 m2 = inpMat * QMatrix4x4((float*)&m);

	shader->setUniformValue("ModelMatrix", m2);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = m_Scene->mMeshes[nd->mMeshes[n]];
		ApplyMaterial(shader, sc->mMaterials[mesh->mMaterialIndex]);

		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices)
			{
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);
			for (i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if (mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
				if (mesh->mNormals != NULL)

					if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}
			glEnd();
		}
	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		RecursiveRender(shader, m2, sc, nd->mChildren[n]);
	}
}

void Model::ApplyMaterial(QOpenGLShaderProgram* shader, const aiMaterial *mtl)
{
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture
	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		auto tex = m_TextureIdMap[texPath.data];
		if (tex)
		{
			tex->bind(0);
			shader->setUniformValue("hasTexture", 1.0f);
			shader->setUniformValue("diffuseTexture", 0);
		}
		else
			shader->setUniformValue("hasTexture", 0.0f);
	}
	else
		shader->setUniformValue("hasTexture", 0.0f);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		shader->setUniformValue("diffuseColor", QVector4D(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	else
		shader->setUniformValue("diffuseColor", QVector4D(0.8f,0.8f,0.8f,1.0f));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		shader->setUniformValue("specularColor", QVector4D(specular.r, specular.g, specular.b, specular.a));
	else
		shader->setUniformValue("specularColor", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		shader->setUniformValue("ambientColor", QVector4D(ambient.r, ambient.g, ambient.b, ambient.a));
	else
		shader->setUniformValue("ambientColor", QVector4D(0.2f, 0.2f, 0.2f, 1.0f));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		shader->setUniformValue("emissiveColor", QVector4D(emission.r, emission.g, emission.b, emission.a));
	else
		shader->setUniformValue("emissiveColor", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

	unsigned int max = 1;
	int ret1, ret2;
	float shininess, strength;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		shader->setUniformValue("shininess", shininess * strength);
	else
		shader->setUniformValue("shininess", 0.0f);

	max = 1;
	int wireframe;
	GLenum fill_mode;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	int two_sided;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}
