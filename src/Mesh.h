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
 *	@file Mesh.h
 */
#pragma once

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLShader>

#define MODEL_VERTEX_LOCATION 0
#define MODEL_NORMAL_LOCATION 1
#define MODEL_TEXCOORD_LOCATION 2
#define MODEL_VERTEX_COLOR_LOCATION 3
#define MODEL_TANGENT_LOCATION 4
#define MODEL_BITANGENT_LOCATION 5
#define MODEL_MATERIAL_LOCATION 6

class Renderer;
class Vertex
{
public:
	Vertex(QVector3D _position, QVector3D _normal, QVector2D _texCoord) :
		position(_position), normal(_normal), texCoord(_texCoord)
	{
		if (normal.length() != 0)
			normal.normalize();

		color = QVector4D(1, 1, 1, 1);
	}

	Vertex(QVector3D _position, QVector3D _normal) : Vertex(_position, _normal, QVector2D())
	{}

	Vertex(QVector3D _position) : Vertex(_position, QVector3D())
	{}

	Vertex(QVector3D _position, QVector3D _normal, QVector2D _texCoord, QVector4D _color) :Vertex(_position, _normal, _texCoord)
	{
		color = _color;
	}

	Vertex() : position(0,0,0), normal(0,0,0), texCoord(0,0)
	{
		color = QVector4D(1, 1, 1, 1);
	}
	
	QVector3D position; //Position of the vertex
	QVector3D normal; //Normal of the vertex
	QVector2D texCoord; //Texture coordinate of the vertex
	QVector4D color; //Color of the vertex
private:
};

class Face
{
public:
	Face(unsigned int a, unsigned int b, unsigned int c)
	{
		Indices[0] = a;
		Indices[1] = b;
		Indices[2] = c;
	}
	unsigned int Indices[3];
};

//typedefs for easier usage

//list of vertices for mesh creation
typedef QVector<Vertex> VertexList;

//list of faces (indices) for mesh creation
typedef QVector<Face> FaceList;

/*class Mesh
{
	friend class NoWork;
	friend class Renderer;
public:
	enum DataUsage
	{
		STREAM_DRAW = 0x88E0,
		STREAM_READ = 0x88E1,
		STREAM_COPY = 0x88E2,
		STATIC_DRAW = 0x88E4,
		STATIC_READ = 0x88E5,
		STATIC_COPY = 0x88E6,
		DYNAMIC_DRAW = 0x88E8,
		DYNAMIC_READ = 0x88E9,
		DYNAMIC_COPY = 0x88EA
	};

	enum RenderMode
	{
		POINTS   	   = 0x0000,
		LINES          = 0x0001,
		LINE_LOOP      = 0x0002,
		LINE_STRIP     = 0x0003,
		TRIANGLES      = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN   = 0x0006,
	};

	~Mesh();


	//Creates a mesh without indices. The mesh must be triangles
	static Mesh* Create(const VertexList &vertices, DataUsage usage = DataUsage::STATIC_DRAW);

	//Creates a mesh with indices. The mesh must be triangles
	static Mesh* Create(const VertexList &vertices, const FaceList &faces, bool calculateNormals = false, DataUsage usage = DataUsage::STATIC_DRAW);
	static Mesh* CreatePlane(DataUsage usage = DataUsage::STATIC_DRAW);

	void Render(QOpenGLShader* shader, RenderMode mode = TRIANGLES);

	VertexList* GetVertexList() { return &m_Vertices; }
	FaceList* GetFacesList() { return &m_Faces; }

	void UpdateBufferData(bool reallocate = false);

protected:
	static void Init(Renderer* renderer);
	Mesh();

	bool CreateVBO();

private:
	void CalculateNormals();


	Renderer* m_Renderer;

	unsigned int m_NumIndices, m_NumVertices;

	DataUsage m_DataUsage;
	QVector<Vertex> m_Vertices;
	QVector<Face> m_Faces;

	unsigned int m_VertexArrayObject;
	unsigned int m_IndexBuffer, m_VertexBuffer, m_NormalBuffer, m_TexCoordBuffer;


};*/