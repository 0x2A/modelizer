/*
	Modelizer - Model file converter
	Copyright (C) 2016 Frank Köhnke

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
 *	@file FBXExporter.h
 */

#pragma once

#include <assimp/types.h>
#include <assimp/version.h>
#include <assimp/IOSystem.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>



// ------------------------------------------------------------------------------------------------
// Worker function for exporting a scene to Autodesk FBX.
void ExportSceneFbx(const char* pFile, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties);

enum FbxVersion
{
	Fbx_2014 = 0xe81c0000,
};
class FbxExporter
{
public:

	/// Constructor for a specific scene to export
	FbxExporter(const char* filename, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties);


private:

	void WriteHeader(Assimp::IOStream* fStream);

	int m_Version;
};