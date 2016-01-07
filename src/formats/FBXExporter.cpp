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
 *	@file FBXExporter.cpp
 */

#include "FBXExporter.h"
#include <assimp/IOStream.hpp>

void ExportSceneFbx(const char* pFile, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties)
{
	// invoke the exporter
	FbxExporter exporter(pFile, pIOSystem, pScene, pProperties);
}

FbxExporter::FbxExporter(const char* filename, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties)
{
	m_Version = Fbx_2014;
	Assimp::IOStream* fStream = pIOSystem->Open(filename,"wb");
	int version = pProperties->GetPropertyInteger("version");
	if (version != -1)
		m_Version = version;

	WriteHeader(fStream);
	pIOSystem->Close(fStream);
}

void FbxExporter::WriteHeader(Assimp::IOStream* fStream)
{
	assert(fStream);

	//write identifier
	const char* buff = "Kaydara FBX Binary  ";
	fStream->Write(buff, 21, 1);
	
	//write magic value
	const unsigned char magic[] = { 0x1A, 0x00 };
	fStream->Write(magic, 2, 1);

	//write version
	unsigned char version[4];
	version[0] = (m_Version >> 24) & 0xFF;
	version[1] = (m_Version >> 16) & 0xFF;
	version[2] = (m_Version >> 8) & 0xFF;
	version[3] = m_Version & 0xFF;
	fStream->Write(version, 4, 1);
}
