#pragma once

#include <QString>
#include <QVector>
class Model
{

public:

	~Model();
	static Model* Load(const QString path);

	void Render();

protected:

	Model();

private:

	//QVector<Mesh*> m_Meshes;
	//QVector<Texture2D*> m_Textures;
	QVector<int> m_MaterialIndices;
	int m_TotalVertices;
};