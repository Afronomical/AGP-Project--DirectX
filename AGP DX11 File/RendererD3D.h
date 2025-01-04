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


//Renderer handles UI, World, GameObjects, Camera and Skybox

using namespace DirectX;

class RendererD3D
{

private:

	static RendererD3D* instance;


	ID3D11Device*			dev = NULL;			//pointer to our direct3d device interface
	ID3D11DeviceContext*	devCon = NULL; // pointer to our Direct 3d device context

	IDXGISwapChain*			swapChain = NULL; //pointer to swap chain interface
	ID3D11RenderTargetView* backBuffer = NULL;
	ID3D11DepthStencilView* zBuffer = NULL;


	ID3D11BlendState*		pAlphaBlendStateEnable = NULL;
	ID3D11BlendState*		pAlphaBlendStateDisable = NULL;


	XMMATRIX world, view, projection;

	HRESULT InitD3D(HWND hWnd);
public:

	HRESULT Initialize(HWND hWnd);
	void Render();

	static RendererD3D* GetInstance() {
		if (!instance) {
			instance = new RendererD3D();
		}
		return instance;
	}
};

