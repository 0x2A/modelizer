#pragma once

#include <QString>
#include <QVector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

class Model
{

public:

	~Model();
	static Model* Load(const QString path, unsigned int flags);
	void Render(QOpenGLShaderProgram* shader);

protected:

	Model();
	bool LoadGLTextures();

private:

	void RecursiveRender(QOpenGLShaderProgram* shader, QMatrix4x4 inpMat, const struct aiScene *sc, const struct aiNode *nd);
	void ApplyMaterial(QOpenGLShaderProgram* shader, const aiMaterial *mtl);

	//QVector<Mesh*> m_Meshes;
	//QVector<Texture2D*> m_Textures;
	const aiScene *m_Scene;
	Assimp::Importer m_Importer;

	std::map<std::string, QOpenGLTexture*> m_TextureIdMap;	// map image filenames to textureIds
	int m_TotalVertices;
	
	QString m_BasePath;

	QMatrix4x4 m_ModelMatrix;
};