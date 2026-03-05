#pragma once
#include <d3d11.h>
#include <iostream>
#include "Buffers.h"

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device): cBufferINCB(nullptr)
	{
		if (!device) {
			std::cerr << "Device is null!" << std::endl;
			return;
		}
		D3D11_BUFFER_DESC cBufferDesc = {};

		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		cBufferDesc.ByteWidth = (sizeof(T) + 15) & ~15;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = 0;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		HRESULT hr = device->CreateBuffer(&cBufferDesc, nullptr, &cBufferINCB);

		if (FAILED(hr)) {
			std::cerr << "Failed to create constant buffer!" << std::endl;
		}
	}

	~ConstantBuffer()
	{
		if (cBufferINCB) cBufferINCB->Release();
	};

	void Update(ID3D11DeviceContext* context, const T& data)
	{
		if (!cBufferINCB) {
			std::cerr << "Constant buffer is not initialized!" << std::endl;
			return;
		}
		context->UpdateSubresource(cBufferINCB, 0, nullptr, &data, 0, 0);
		context->VSSetConstantBuffers(0, 1, &cBufferINCB);
		context->PSSetConstantBuffers(0, 1, &cBufferINCB);
	}

	ID3D11Buffer* GetBuffer() const { return cBufferINCB; }

private:
	ID3D11Buffer* cBufferINCB;

};

