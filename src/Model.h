#pragma once

#include <QString>
#include <QVector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLTexture>

class Model
{

public:

	~Model();
	static Model* Load(const QString path, unsigned int flags);

	void Render();

protected:

	Model();
	bool LoadGLTextures();

private:

	//QVector<Mesh*> m_Meshes;
	//QVector<Texture2D*> m_Textures;
	const aiScene *m_Scene;
	Assimp::Importer m_Importer;

	std::map<std::string, QOpenGLTexture*> m_TextureIdMap;	// map image filenames to textureIds
	int m_TotalVertices;
	
	QString m_BasePath;
};