#include "MeshManager.h"
#include <iostream>

MeshManager* MeshManager::instance = nullptr;

void MeshManager::Init(ID3D11Device* dev, ID3D11DeviceContext* devCon)
{
	device = dev;
	context = devCon;
	MaterialManager::GetInstance()->Init(dev, devCon);
	LoadMeshes();
	std::cout << "MeshManager initialized" << std::endl;
}

HRESULT MeshManager::CreateMesh(std::string meshName, std::string materialName, const char* objFileName)
{
	if (MeshLibrary.find(meshName) == MeshLibrary.end())
	{
		Mesh* newMesh = new Mesh();
		newMesh->material = MaterialManager::GetInstance()->GetMaterial(materialName);
		newMesh->fileModel = new ObjFileModel{ (char*)objFileName, device, context };

		if (newMesh->fileModel->vertices == NULL)
		{
			std::cerr << "MeshManager::CreateMesh Failed to create mesh. *" << meshName << "* mesh could not load ObjFileModel" << endl;
			return E_FAIL;
		}

		MeshLibrary.insert({ meshName, newMesh });
		std::cout << "MeshManager::CreateMesh successfully created Mesh named: *" + meshName + "*" << endl;
		return S_OK;
	}
	else
	{
		std::cerr << "MeshManager::CreateMesh Failed to create mesh. *" << meshName <<  "* already exists" << endl;
		return E_FAIL;
	}


	
}

Mesh* MeshManager::GetMesh(const std::string& meshName)
{
	if (MeshLibrary.find(meshName) != MeshLibrary.end())
	{
		return MeshLibrary[meshName];
	}
	else
	{
		std::cout << "MeshManager::GetMesh No mesh of type *" + meshName + "* was found in MeshLibrary" << endl;
		return MeshLibrary["Default"];
	}
}

void MeshManager::CleanAllMeshes()
{
	for (auto& mesh : MeshLibrary)
	{
		delete mesh.second;
		mesh.second = nullptr;
	}

}


