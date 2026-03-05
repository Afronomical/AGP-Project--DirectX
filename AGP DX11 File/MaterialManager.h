#pragma once

#include <d3d11.h>
#include <string>
#include <map>

#include "Material.h"

enum MaterialTypes
{
    mat_GameObject,
    mat_Skybox,
    mat_Wave,
};
class MaterialManager
{
private:

    std::map<std::string, Material*> MaterialLibrary;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    
public:
    ~MaterialManager() {
		CleanAllMaterials();
		if (device) device->Release();
		if (context) context->Release();
    }
    void Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);

    HRESULT CreateMaterial(std::string MaterialName, LPCWSTR textureFileLocation, std::string shaderName, bool isSkybox);
    Material* GetMaterial(const std::string& materialName);

    void CleanAllMaterials();

    void LoadMaterials()
    {
        CreateMaterial("ErrorNoMaterial",L"Textures/Error.png",                 "Emissive",    false);
       
        CreateMaterial("Skybox",        L"SkyboxResources/skybox02.dds",        "Skybox",      true);
        CreateMaterial("DebugSkybox",   L"SkyboxResources/Debug.dds",           "Skybox",      true);
        CreateMaterial("Space",         L"SkyboxResources/NightSky.dds",        "Skybox",      true);
       
        CreateMaterial("Standard",      L"Textures/StandardCheckered.jpg",      "Standard",    false);
        CreateMaterial("Box",           L"Textures/Box.bmp",                    "Standard",    false);
        CreateMaterial("Reflective",    L"Textures/Box.bmp",                    "Reflective",  false);
        
        
        CreateMaterial("WaveMaterial",  L"Textures/box.bmp",                    "WaveShader",  false);
        CreateMaterial("Explosion",     L"Textures/explode.bmp",                "Standard",    false);

		CreateMaterial("SunMat",        L"Textures/Planets/Sunmap.jpg",         "Emissive", false);
		CreateMaterial("MercuryMat",    L"Textures/Planets/mercurymap.jpg",     "Standard", false);
		CreateMaterial("EarthMat",      L"Textures/Planets/earthmap.jpg",       "Standard", false);
		CreateMaterial("MarsMat",       L"Textures/Planets/marsmap.jpg",        "Standard", false);
		CreateMaterial("NeptuneMat",    L"Textures/Planets/neptunemap.jpg",     "Standard", false);
		CreateMaterial("VenusMat",      L"Textures/Planets/Venusmap.jpg",       "Standard", false);
		CreateMaterial("MoonMat",       L"Textures/Planets/Plutomap.jpg",       "Standard", false);
		CreateMaterial("JupiterMat",    L"Textures/Planets/Jupitermap.jpg",     "Standard", false);
        CreateMaterial("SaturnMat",     L"Textures/Planets/Saturnmap.jpg",      "Standard", false);
		CreateMaterial("UranusMat",     L"Textures/Planets/Uranusmap.jpg",      "Standard", false);



		CreateMaterial("Missile",       L"Textures/aim120.png",                 "Standard", false);
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

