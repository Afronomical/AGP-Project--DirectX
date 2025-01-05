#pragma once
#include <d3d11.h>
#include <string>
class ShaderManager
{
private:
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;

    ID3D11Device* device;
    ID3D11DeviceContext* context;

    HRESULT LoadVertexShader(ID3D11Device* dev, ID3D11DeviceContext* devCon, LPCWSTR fileName, LPCSTR entryPoint, ID3D11VertexShader** vs, ID3D11InputLayout** i1);
    HRESULT LoadPixelShader(ID3D11Device* dev, ID3D11DeviceContext* devCon, LPCWSTR filename, LPCSTR entryPoint, ID3D11PixelShader** ps);
public:
    ~ShaderManager();

    HRESULT CreateShaders(ID3D11Device* device, ID3D11DeviceContext* context);
    void UseShader(const std::string& shaderName);

#pragma region Singleton Pattern

private:
    static ShaderManager* instance;
    ShaderManager() {};
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

