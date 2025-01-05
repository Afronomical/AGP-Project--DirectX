#include "ShaderManager.h"
#include <d3dcompiler.h>

HRESULT ShaderManager::LoadVertexShader(ID3D11Device* dev, ID3D11DeviceContext* devCon, LPCWSTR fileName, LPCSTR entryPoint, ID3D11VertexShader** vs, ID3D11InputLayout** i1)
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
	result = dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, vs);
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
	result = dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), i1);
	VS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	return S_OK;
}

HRESULT ShaderManager::LoadPixelShader(ID3D11Device* dev, ID3D11DeviceContext* devCon, LPCWSTR filename, LPCSTR entryPoint, ID3D11PixelShader** ps)
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

	result = dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, ps);
	PS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create pixel shader");
		return result;
	}
	return S_OK;
}
