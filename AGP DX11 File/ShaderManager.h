#pragma once
#include <d3d11.h>
#include <string>
#include <map>
#include "Shader.h"


class ShaderManager
{
private:
    
    std::map<std::string, Shader*> ShaderLibrary;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    HRESULT LoadVertexShader( LPCWSTR fileName, LPCSTR entryPoint, ID3D11VertexShader** vs, ID3D11InputLayout** i1);
    HRESULT LoadPixelShader( LPCWSTR filename, LPCSTR entryPoint, ID3D11PixelShader** ps);
public:
    ~ShaderManager();
    void Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);

    HRESULT CreateShader(std::string ShaderName, LPCWSTR vsfileName, LPCWSTR psFileName, LPCSTR entryPoint);
    Shader* GetShader(const std::string& shaderName);

    void CleanAllShaders();

    void LoadShaders()
    {

        CreateShader("Default",         L"VertexShader.hlsl",       L"PixelShader.hlsl",        "main");
        CreateShader("Reflective",      L"ReflectiveVShader.hlsl",  L"ReflectivePShader.hlsl",  "main");
        CreateShader("Skybox",          L"SkyboxVShader.hlsl",      L"SkyboxPShader.hlsl",      "main");

    }




#pragma region Singleton Pattern

private:
    static ShaderManager* instance;
    ShaderManager() { };
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
public:
    static ShaderManager* GetInstance() {
        if (!instance) {
            instance = new ShaderManager();
        }
        return instance;
    }

#pragma endregion
};

