#pragma once

#include <d3d11.h>
#include <string>
#include <map>

#include "Material.h"
class MaterialManager
{
private:

    std::map<std::string, Material*> MaterialLibrary;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    
public:
    ~MaterialManager();
    void Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);

    HRESULT CreateMaterial(std::string MaterialName, LPCWSTR textureFileLocation, std::string shaderName, bool isSkybox);
    Material* GetMaterial(const std::string& materialName);

    void CleanAllMaterials();

    void LoadMaterials()
    {
        CreateMaterial("Skybox", L"SkyboxResources/skybox01.dds", "Skybox", true);
        CreateMaterial("Default", L"Box.bmp", "Default", false);
        //CreateMaterial("Reflective", L"Box.bmp", "Default", false);
    }
    




#pragma region Singleton Pattern

private:
    static MaterialManager* instance;
    MaterialManager() { };
    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;
public:
    static MaterialManager* GetInstance() {
        if (!instance) {
            instance = new MaterialManager();
        }
        return instance;
    }

#pragma endregion
};

