#include "MaterialManager.h"
#include "WICTextureLoader.h"
#include <DDSTextureLoader.h>
#include "RendererD3D.h"
#include "ShaderManager.h"


MaterialManager* MaterialManager::instance = nullptr;

void MaterialManager::Init(ID3D11Device* dev, ID3D11DeviceContext* devCon)
{
	ShaderManager::GetInstance()->Init(dev, devCon);
	LoadMaterials();
	std::cout << "MaterialManager initialized" << std::endl;
}

HRESULT MaterialManager::CreateMaterial(std::string materialName, LPCWSTR textureFileLocation, std::string shaderName, bool isSkybox)
{
	if (MaterialLibrary.find(materialName) == MaterialLibrary.end())
	{
		Material* newMtl = new Material();
		if (isSkybox)
		{
			CreateDDSTextureFromFile(RendererD3D::GetInstance()->GetDevice(), RendererD3D::GetInstance()->GetDeviceContext(), textureFileLocation, NULL, &newMtl->texture);
		}
		else
		{
			CreateWICTextureFromFile(RendererD3D::GetInstance()->GetDevice(), RendererD3D::GetInstance()->GetDeviceContext(), textureFileLocation, NULL, &newMtl->texture);
		}

		newMtl->shader = ShaderManager::GetInstance()->GetShader(shaderName);

		if (newMtl->texture == nullptr)
		{
			std::cerr << "MaterialManager::CreateMaterial: Failed to create material. Missing texture" << endl;
			return E_FAIL;
		}
		if (newMtl->shader == nullptr)
		{
			std::cerr << "MaterialManager::CreateMaterial: Failed to create material. Missing shader" << endl;
			return E_FAIL;
		}

		MaterialLibrary.insert({ materialName, newMtl });
		std::cout << "MaterialManager::CreateMaterial: successfully created material named: *" + materialName + "*" << endl;
		return S_OK;
	}
	else
	{
		std::cerr << "MaterialManager::CreateMaterial: Failed to create material. Error: *" + materialName + "* was already found" << endl;
		return E_FAIL;
	}
	
}

Material* MaterialManager::GetMaterial(const std::string& materialName)
{
	if (MaterialLibrary.find(materialName) != MaterialLibrary.end())
	{
		return MaterialLibrary[materialName];
	}
	else
	{
		std::cout << "MaterialManager::GetMaterial No material of type *" + materialName + "* was found in MaterialLibrary" << endl;
		return MaterialLibrary["Default"];
	}
}

void MaterialManager::CleanAllMaterials()
{
	for (auto material : MaterialLibrary)
	{
		delete material.second;
		material.second = nullptr;
	}
}
