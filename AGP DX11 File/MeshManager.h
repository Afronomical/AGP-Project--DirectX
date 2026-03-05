#pragma once
#include <d3d11.h>
#include <map>
#include "Mesh.h"
#include "string"
#include "MaterialManager.h"


class MeshManager
{
private:

    std::map<std::string, Mesh*> MeshLibrary;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    
public:
    ~MeshManager() {
        CleanAllMeshes();

    }
    void Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);

    HRESULT CreateMesh(std::string meshName, std::string materialName, const char* objFileName );
    Mesh* GetMesh(const std::string& meshName);

    

    void CleanAllMeshes();

    void LoadMeshes()
    {
        CreateMesh("ErrorNoMesh", "ErrorNoMaterial", "cube.obj");

        CreateMesh("Skybox", "Skybox", "Meshes/cube.obj");
        CreateMesh("Space", "Space", "Meshes/cube.obj");
        CreateMesh("DebugSkybox", "DebugSkybox", "Meshes/Sphere.obj");

        CreateMesh("Standard", "Standard", "Meshes/cube.obj");
        CreateMesh("Sphere", "Skybox", "Meshes/sphere.obj");
        CreateMesh("Cube", "Box", "Meshes/cube.obj");

        CreateMesh("Reflective", "Reflective", "Meshes/cube.obj");
        CreateMesh("WaveMesh", "WaveMaterial", "Meshes/cube.obj");

        CreateMesh("Sun", "SunMat", "Meshes/sphere.obj");
        CreateMesh("Mercury", "MercuryMat", "Meshes/sphere.obj");
		CreateMesh("Earth", "EarthMat", "Meshes/sphere.obj");
		CreateMesh("Mars", "MarsMat", "Meshes/sphere.obj");
		CreateMesh("Neptune", "NeptuneMat", "Meshes/sphere.obj");
		CreateMesh("Venus", "VenusMat", "Meshes/sphere.obj");
		CreateMesh("Moon", "MoonMat", "Meshes/sphere.obj");

		CreateMesh("Jupiter", "JupiterMat", "Meshes/sphere.obj");
		CreateMesh("Saturn", "SaturnMat", "Meshes/sphere.obj");
		CreateMesh("Uranus", "UranusMat", "Meshes/sphere.obj");


        CreateMesh("Missile", "Missile", "Meshes/aim120.obj");

    }

    
#pragma region Singleton Pattern

private:
    static MeshManager* instance;
    MeshManager() {};
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;
public:
    static MeshManager* GetInstance() {
        if (!instance) {
            instance = new MeshManager();
        }
        return instance;
    }

#pragma endregion
};

