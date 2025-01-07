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
    ~MeshManager();
    void Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);

    HRESULT CreateMesh(std::string meshName, std::string materialName, LPCWSTR objFileName );
    Mesh* GetMesh(const std::string& meshName);

    void CleanAllMeshes();

    void LoadMeshes();

    
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

