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
#include "../modelizer.h"

#ifndef MODELIZER_BUILD_NO_FBX_EXPORTER

#if defined(_MSC_VER)
#pragma comment(lib, "libfbxsdk-md.lib")
#endif

#define IOS_REF (*(m_SdkManager->GetIOSettings()))


void ExportSceneFbx(const char* pFile, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties)
{
	// invoke the exporter
	mFbxExporter exporter(pFile, pIOSystem, pScene, pProperties);
}

mFbxExporter::mFbxExporter(const char* filename, Assimp::IOSystem* pIOSystem, const aiScene* pScene, const Assimp::ExportProperties* pProperties)
{
	m_SdkManager = FbxManager::Create();
	if (!m_SdkManager)
	{
		modelizer::Log->AppendError("Unable to create FBX Manager!");
		return;
	}

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(m_SdkManager, IOSROOT);
	m_SdkManager->SetIOSettings(ios);

	//Load plugins from the executable directory
	FbxString lPath = FbxGetApplicationDirectory();
	m_SdkManager->LoadPluginsDirectory(lPath.Buffer());

	
	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	m_Scene = FbxScene::Create(m_SdkManager, "Scene");
	if (!m_Scene)
	{
		modelizer::Log->AppendError("Unable to create FBX scene!\n");
		return;
	}

	FbxDocumentInfo* sceneInfo = FbxDocumentInfo::Create(m_SdkManager, "SceneInfo");
	sceneInfo->mTitle = pProperties->GetPropertyString("title").c_str();
	sceneInfo->mAuthor = pProperties->GetPropertyString("author").c_str();
	sceneInfo->mSubject = pProperties->GetPropertyString("subject").c_str();
	sceneInfo->mRevision = pProperties->GetPropertyString("revision").c_str();
	sceneInfo->mKeywords = pProperties->GetPropertyString("keywords").c_str();
	sceneInfo->mComment = "File exported by modelizer v" + FbxString(MODELIZER_VERSION_STR);
	m_Scene->SetSceneInfo(sceneInfo);

	AddMaterials(pScene);
	AddNode(m_Scene->GetRootNode(), pScene, pScene->mRootNode);

	SaveScene(filename, -1, false);

	m_Scene->Destroy(true);
	m_SdkManager->Destroy();
}


void mFbxExporter::AddNode(FbxNode* parent, const aiScene* scene, aiNode* node)
{
	FbxNode* fbxNode = FbxNode::Create(m_SdkManager, node->mName.C_Str());
	parent->AddChild(fbxNode);

	FbxAMatrix &transform = fbxNode->EvaluateLocalTransform();
	AiMatrixToFbxMatrix(transform, node->mTransformation);
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
	{
		auto mesh = scene->mMeshes[node->mMeshes[i]];
		FbxNode* meshNode = FbxNode::Create(m_SdkManager, mesh->mName.C_Str());
		fbxNode->AddChild(meshNode);
		AddMesh(meshNode, mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		/*FbxNode* chld = FbxNode::Create(m_SdkManager, "");
		fbxNode->AddChild(chld);*/
		AddNode(fbxNode, scene, node->mChildren[i]);
	}
}

void mFbxExporter::AiMatrixToFbxMatrix(FbxAMatrix &fbxMat, aiMatrix4x4 aiMat)
{
	fbxMat.SetRow(0, FbxVector4(aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4));
	fbxMat.SetRow(1, FbxVector4(aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4));
	fbxMat.SetRow(2, FbxVector4(aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4));
	fbxMat.SetRow(3, FbxVector4(aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4));
}

void mFbxExporter::AddMesh(FbxNode* fbxNode, const aiMesh* m)
{
	FbxMesh* mesh = FbxMesh::Create(m_SdkManager, m->mName.C_Str());
	
	mesh->InitControlPoints(m->mNumVertices);
	FbxVector4* lControlPoints = mesh->GetControlPoints();
	for (int i = 0; i < m->mNumVertices; i++)
	{
		auto aiVertex = m->mVertices[i];
		lControlPoints[i] = FbxVector4(aiVertex.x, aiVertex.y, aiVertex.z);
	}

	for (int i = 0; i < m->mNumFaces; i++)
	{
		const aiFace& f = m->mFaces[i];
		mesh->BeginPolygon(m->mMaterialIndex);
		for (int j = 0; j < f.mNumIndices; j++)
		{
			mesh->AddPolygon(f.mIndices[j]);
		}
		mesh->EndPolygon();
	}

	if (m->HasNormals())
	{
		// We want to have one normal for each vertex (or control point),
		// so we set the mapping mode to eByControlPoint.
		FbxGeometryElementNormal* lGeometryElementNormal = mesh->CreateElementNormal();
		lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
		
		// set the actual normal value for every control point.
		lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
		for (int i = 0; i < m->mNumVertices; i++)
		{
			auto aiNormal = m->mNormals[i];
			lGeometryElementNormal->GetDirectArray().Add(FbxVector4(aiNormal.x, aiNormal.y, aiNormal.z));
		}
	}

	if (m->HasTextureCoords(0))
	{
		// Create UV for Diffuse channel
		FbxGeometryElementUV* lUVDiffuseElement = mesh->CreateElementUV("UVW");
		FBX_ASSERT(lUVDiffuseElement != NULL);
		lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);
		for (int j = 0; j < m->mNumVertices; j++)
		{
			auto uv = m->mTextureCoords[0][j];
			lUVDiffuseElement->GetDirectArray().Add(FbxVector2(uv.x, uv.y));
		}
	}
	fbxNode->AddMaterial(m_Scene->GetMaterial(m->mMaterialIndex));
	
	FbxStatus status;
	if (!fbxNode->AddNodeAttribute(mesh, &status))
	{
		modelizer::Log->AppendError(status.GetErrorString());
	}
}

std::string mFbxExporter::GetMaterialName(const aiScene* scene, unsigned int index)
{
	const aiMaterial* const mat = scene->mMaterials[index];
	aiString s;
	if (AI_SUCCESS == mat->Get(AI_MATKEY_NAME, s)) {
		return std::string(s.data, s.length);
	}

	char number[sizeof(unsigned int) * 3 + 1];
	itoa(index, number,10);
	return "$Material_" + std::string(number);
}

void mFbxExporter::AddMaterials(const aiScene* scene)
{
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		auto mat = scene->mMaterials[i];
		FbxSurfacePhong *fbxmat = FbxSurfacePhong::Create(m_SdkManager, GetMaterialName(scene, i).c_str());
		aiColor4D c;
		if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, c)) {
			fbxmat->Diffuse.Set(FbxDouble3(c.r, c.g, c.b));
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, c)) {
			fbxmat->Ambient.Set(FbxDouble3(c.r, c.g, c.b));
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, c)) {
			fbxmat->Specular.Set(FbxDouble3(c.r, c.g, c.b));
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_EMISSIVE, c)) {
			fbxmat->Emissive.Set(FbxDouble3(c.r, c.g, c.b));
		}

		float o;
		if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, o)) {
			fbxmat->Shininess.Set(o);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_OPACITY, o)) {
			fbxmat->TransparencyFactor.Set(o);
		}
		aiString s;
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), s)) {
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Diffuse texture");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->Diffuse.ConnectSrcObject(tex);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_AMBIENT(0), s)) {
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Ambient texture");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->Ambient.ConnectSrcObject(tex);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_SPECULAR(0), s)) {
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Specular texture");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->Specular.ConnectSrcObject(tex);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_SHININESS(0), s)) {
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Shininess texture");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->Shininess.ConnectSrcObject(tex);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_OPACITY(0), s)) {
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Opacity texture");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->TransparencyFactor.ConnectSrcObject(tex);
		}
		if (AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_HEIGHT(0), s) || AI_SUCCESS == mat->Get(AI_MATKEY_TEXTURE_NORMALS(0), s))
		{
			FbxFileTexture *tex = FbxFileTexture::Create(m_SdkManager, "Normal map");
			tex->SetFileName(s.C_Str());
			tex->SetMappingType(FbxTexture::eUV);
			tex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			tex->SetTranslation(0.0, 0.0);
			tex->SetScale(1.0, 1.0);
			tex->SetRotation(0.0, 0.0);
			fbxmat->Bump.ConnectSrcObject(tex);
		}

		m_Scene->AddMaterial(fbxmat);
	}

	m_Scene->ConnectMaterials();
}

void mFbxExporter::SaveScene(const char* pFilename, int pFileFormat, bool pEmbedMedia)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(m_SdkManager, "");
	if (pFileFormat < 0 || pFileFormat >= m_SdkManager->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// Write in fall back format in less no ASCII format found
		pFileFormat = m_SdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

		//Try to export in ASCII if possible
		int lFormatIndex, lFormatCount = m_SdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

		for (lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
		{
			if (m_SdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{
				FbxString lDesc = m_SdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *lASCII = "FBX binary";
				if (lDesc.Find(lASCII) >= 0)
				{
					pFileFormat = lFormatIndex;
					break;
				}
			}
		}
	}

	// Set the export states. By default, the export states are always set to 
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
	// shows how to change these states.
	IOS_REF.SetBoolProp(EXP_FBX_MATERIAL, true);
	IOS_REF.SetBoolProp(EXP_FBX_TEXTURE, true);
	IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED, pEmbedMedia);
	IOS_REF.SetBoolProp(EXP_FBX_SHAPE, true);
	IOS_REF.SetBoolProp(EXP_FBX_GOBO, true);
	IOS_REF.SetBoolProp(EXP_FBX_ANIMATION, true);
	IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Initialize the exporter by providing a filename.
	if (lExporter->Initialize(pFilename, pFileFormat, m_SdkManager->GetIOSettings()) == false)
	{
		modelizer::Log->AppendError("Call to FbxExporter::Initialize() failed.");
		modelizer::Log->AppendError("Error returned: " + QString(lExporter->GetStatus().GetErrorString()));
		return;
	}

	
	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	modelizer::Log->AppendMessage(QString("FBX file format version %1.%2.%3\n\n").arg(lMajor).arg(lMinor).arg(lRevision));

	// Export the scene.
	lStatus = lExporter->Export(m_Scene);

	// Destroy the exporter.
	lExporter->Destroy();
}

#endif