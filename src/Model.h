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
 *	@file Model.h
 */
#pragma once

#include <QString>
#include <QVector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

class Model
{

public:

	~Model();
	static Model* Load(const QString path, unsigned int flags);
	
	void Export(const QString path, const QString formatId, unsigned int flags);
	void Render(QOpenGLShaderProgram* shader);

	Assimp::Exporter* GetExporter() { return &m_Exporter; }

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
	Assimp::Exporter m_Exporter;


	std::map<std::string, QOpenGLTexture*> m_TextureIdMap;	// map image filenames to textureIds
	int m_TotalVertices;
	
	QString m_BasePath;

	QMatrix4x4 m_ModelMatrix;
};