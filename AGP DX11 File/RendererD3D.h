#pragma once


#include <d3d11.h>
#include <iostream>

#include <DirectXColors.h>
#include <DirectXMath.h>

//#include <d3d11shader.h>
#include <d3dcompiler.h>

#include "ReadData.h"
//#include <WICTextureLoader.h>
#include "text2D.h"
#include "objfilemodel.h"
#include "Skybox.h"

#include "Camera.h"

#include "Scene.h"


//Renderer handles UI, World, GameObjects, Camera and Skybox

using namespace DirectX;

class RendererD3D
{

private:


	ID3D11RasterizerState* rasterSolid = nullptr;
	ID3D11RasterizerState* rasterSkybox = nullptr;

	ID3D11DepthStencilState* depthWriteEnabled = nullptr;
	ID3D11DepthStencilState* depthWriteDisabled = nullptr;

	static RendererD3D* instance;
	FLOAT bgColor[4]{ 1.0f, 0.4f, 0.6f, 1.0f };

	ID3D11Device* dev = nullptr;			//pointer to our direct3d device interface
	ID3D11DeviceContext* devCon = nullptr; // pointer to our Direct 3d device context

	IDXGISwapChain* swapChain = nullptr; //pointer to swap chain interface
	ID3D11RenderTargetView* backBuffer = nullptr;
	ID3D11DepthStencilView* zBuffer = nullptr;


	ID3D11BlendState* pAlphaBlendStateEnable = nullptr;
	ID3D11BlendState* pAlphaBlendStateDisable = nullptr;
	ID3D11SamplerState* sampler = nullptr;


	HRESULT InitD3D(HWND hWnd);
public:


	
	Camera* cam;
	HRESULT Initialize(HWND hWnd, Scene* scene);

	void RenderFrame(Scene* scene);
	void DrawMesh(Mesh* mesh, ID3D11Buffer* constantBuffer, void* cbufferData, size_t cbufferSize);
	
	void Clean();
	
	
	
	ID3D11Device* GetDevice() { return dev; };
	ID3D11DeviceContext* GetDeviceContext() { return devCon; };
	ID3D11SamplerState* GetSampler() const { return sampler; }

	ID3D11RasterizerState* GetRasterizerSolid() const { return rasterSolid; }
	ID3D11RasterizerState* GetRasterizerSkybox() const { return rasterSkybox; }

	ID3D11DepthStencilState* GetDepthWriteEnabled() const { return depthWriteEnabled; }
	ID3D11DepthStencilState* GetDepthWriteDisabled() const { return depthWriteDisabled; }

	static RendererD3D* GetInstance() {
		if (!instance) {
			instance = new RendererD3D();
		}
		return instance;
	}
};

