#include "ShaderManager.h"
#include <d3dcompiler.h>
#include "RendererD3D.h"

ShaderManager* ShaderManager::instance = nullptr;

HRESULT ShaderManager::LoadVertexShader( LPCWSTR fileName, LPCSTR entryPoint, ID3D11VertexShader** vs, ID3D11InputLayout** i1)
{
	HRESULT result;
	ID3DBlob* VS, * pErrorBlob;
	

	result = D3DCompileFromFile(fileName, 0, 0, entryPoint, "vs_4_0", 0, 0, &VS, &pErrorBlob);
	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}
	result = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, vs);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create vertex shader");
		return result;
	}

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	result = device->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), i1);
	VS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	return S_OK;
}

HRESULT ShaderManager::LoadPixelShader( LPCWSTR filename, LPCSTR entryPoint, ID3D11PixelShader** ps)
{
	HRESULT result;
	ID3DBlob* PS, * pErrorBlob;

	result = D3DCompileFromFile(filename, 0, 0, entryPoint, "ps_4_0", 0, 0, &PS, &pErrorBlob);
	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}

	result = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, ps);
	PS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create pixel shader");
		return result;
	}
	return S_OK;
}


void ShaderManager::Init(ID3D11Device* dev, ID3D11DeviceContext* devCon)
{
	device = dev;
	context = devCon;
	LoadShaders();
	std::cout << "ShaderManager initialized" << std::endl;
}

HRESULT ShaderManager::CreateShader(std::string ShaderName, LPCWSTR vsFileName, LPCWSTR psFileName, LPCSTR entryPoint)
{
	

	if (ShaderLibrary.find(ShaderName) == ShaderLibrary.end())
	{
		Shader* newShader = new Shader();

		HRESULT hrVS = LoadVertexShader(vsFileName, entryPoint, &newShader->vertexShader, &newShader->inputLayout);
		HRESULT hrPS = LoadPixelShader(psFileName, entryPoint, &newShader->pixelShader);

		if (FAILED(hrVS) || FAILED(hrPS))
		{
			std::cerr << "ShaderManager::CreateShader: Failed to load shaders for: *" << ShaderName << "*" << std::endl;
			delete newShader; //Clean up if creation fails
			return E_FAIL;
		}
		ShaderLibrary.insert({ ShaderName, newShader });
		std::cout << "Vertex Shader Address: " << newShader->vertexShader << std::endl;
		std::cout << "Pixel Shader Address: " << newShader->pixelShader << std::endl;
		std::cout << "Input Layout Address: " << newShader->inputLayout << std::endl;
		std::cout << "ShaderManager::CreateShaders: successfully created shader named: *" + ShaderName + "*" << endl;
		return S_OK;
	}
	else
	{
		std::cout << "ShaderManager::CreateShaders: Failed to create shader. Error: *" + ShaderName + "* was already found" << endl;
		return E_FAIL;
	}

	

}

Shader* ShaderManager::GetShader(const std::string& shaderName)
{
	if (ShaderLibrary.find(shaderName) != ShaderLibrary.end())
	{
		std::cout << "returned shader named: " << shaderName << std::endl;
		return ShaderLibrary[shaderName];
	}
	else
	{
		std::cout << "ShaderManager::GetShader: No shader of type *" + shaderName + "* was found in ShadersLibrary" << endl;
		return ShaderLibrary["Emissive"];
	}
	

}

void ShaderManager::CleanAllShaders()
{
	for (auto& shader : ShaderLibrary)
	{
		delete shader.second;
		shader.second = nullptr;
	}


}


