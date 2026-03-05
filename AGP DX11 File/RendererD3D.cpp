#include "RendererD3D.h"
#include "Window.h"
#include "MeshManager.h"


RendererD3D* RendererD3D::instance = nullptr;

HRESULT RendererD3D::Initialize(HWND hWnd, Scene* scene)
{

	HRESULT hr = InitD3D(hWnd);
	if (FAILED(hr)) return hr;
	std::cout << "RendererD3D initialized" << std::endl;
	MeshManager::GetInstance()->Init(dev, devCon);
	Skybox::GetInstance()->Initialize(dev, devCon, scene->skyboxName);
	
	return S_OK;
}



void RendererD3D::RenderFrame(Scene* scene)
{
	swapChain->SetFullscreenState(scene->GetFullScreen(), NULL);
	cam = scene->GetCamera();
	devCon->ClearRenderTargetView(backBuffer, bgColor);
	devCon->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	
	Skybox::GetInstance()->DrawSkyBox();

	XMMATRIX w;
	XMMATRIX v = cam->GetViewMatrix();
	XMMATRIX p = cam->GetProjectionMatrix();

	CBUFFER0 cBufData;
	devCon->PSSetSamplers(0, 1, &sampler);
	
	for (auto& gameObject : scene->gameObjects)
	{
		if (gameObject && gameObject->IsActive() && gameObject->IsVisible())
		{
			w = gameObject->transform->GetWorldMatrix();
			cBufData.WVP = w * v * p;


			cBufData.ambientLightCol = *scene->ambientLightCol;
			cBufData.directionalLightCol = *scene->directionalLightCol;
			XMMATRIX transpose = DirectX::XMMatrixTranspose(w);
			XMVECTOR lightDir = *scene->directionalLightDir;
			cBufData.directionalLightDir = DirectX::XMVector3Transform(lightDir, transpose);

			auto pointLights = scene->pointLights;
			for (int i = 0; i < pointLights.capacity(); ++i)
			{
				if (!pointLights[i]->active)
					continue; //Skip disabled lights

				XMMATRIX inverse = XMMatrixInverse(nullptr, w);

				cBufData.pointLights[i].position = XMVector3Transform(pointLights[i]->position, inverse);
				cBufData.pointLights[i].colour = pointLights[i]->colour;
				cBufData.pointLights[i].strength = pointLights[i]->strength;
				cBufData.pointLights[i].active = pointLights[i]->active;
			}
			//Update cbuffer and draw
			gameObject->Draw(devCon,cBufData);


		}
	}
	if (scene->UI == nullptr)
	{
		std::cerr << "Error RendererD3D: Scene UI is null! \n                 Ensure Scene::Awake is called in override" << std::endl;

	}
	else
	{
		devCon->OMSetBlendState(pAlphaBlendStateEnable, 0, 0xffffffff);
		//Text stuff
		scene->UI->RenderText();
		devCon->OMSetBlendState(pAlphaBlendStateDisable, 0, 0xffffffff);

	}


	

	swapChain->Present(scene->GetVSync(), 0);
}

void RendererD3D::DrawMesh(Mesh* mesh, ID3D11Buffer* constantBuffer, void* cbufferData, size_t cbufferSize)
{

	if (!mesh) {
		std::cerr << "Error RendererD3D: Mesh is null!" << std::endl;
		return;
	}

	if (!mesh->material)
	{
		std::cerr << "Error RendererD3D: Mesh material is null!" << std::endl;
		return;
	}

	if (!mesh->material->shader)
	{
		std::cerr << "Error RendererD3D: Mesh material shader is null!" << std::endl;
		return;
	}

	if (!mesh->material->shader->pixelShader)
	{
		std::cerr << "Error RendererD3D: Mesh pixelShader is null!" << std::endl;
		return;
	}
	devCon->VSSetShader(mesh->material->shader->vertexShader, 0, 0);
	devCon->PSSetShader(mesh->material->shader->pixelShader, 0, 0);
	devCon->IASetInputLayout(mesh->material->shader->inputLayout);
	if (constantBuffer && cbufferData && cbufferSize > 0)
	{
		devCon->UpdateSubresource(constantBuffer, 0, NULL, cbufferData, 0, 0);
		devCon->VSSetConstantBuffers(0, 1, &constantBuffer);
	}
	devCon->PSSetShaderResources(0, 1, &mesh->material->texture);
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh->fileModel->Draw();
}

void RendererD3D::Clean()
{
	if (pAlphaBlendStateEnable) { pAlphaBlendStateEnable->Release(); pAlphaBlendStateEnable = nullptr; }
	if (pAlphaBlendStateDisable) { pAlphaBlendStateDisable->Release(); pAlphaBlendStateDisable = nullptr; }
	if (depthWriteEnabled) { depthWriteEnabled->Release(); depthWriteEnabled = nullptr; }
	if (depthWriteDisabled) { depthWriteDisabled->Release(); depthWriteDisabled = nullptr; }
	if (rasterSolid) { rasterSolid->Release(); rasterSolid = nullptr; }
	if (rasterSkybox) { rasterSkybox->Release(); rasterSkybox = nullptr; }
	if (sampler) { sampler->Release(); sampler = nullptr; }

	if (swapChain) {swapChain->Release(); swapChain = nullptr;}
	if (backBuffer) { backBuffer->Release(); backBuffer = nullptr; }
	if (zBuffer) { zBuffer->Release(); zBuffer = nullptr; }

	if (devCon) { devCon->Release(); devCon = nullptr; }

	Skybox* skybox = Skybox::GetInstance();
	if (skybox) {
		delete skybox;
	}

	MeshManager* meshManager = MeshManager::GetInstance();
	if (meshManager) {
		meshManager->CleanAllMeshes();
		delete meshManager;
	}
	std::cout << "RendererD3D cleaned up" << std::endl;

	if (dev) {
		ID3D11Debug* d3dDebug = nullptr;
		if (SUCCEEDED(dev->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug)))) {
			d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			d3dDebug->Release();
		}
	};
	if (dev) { dev->Release(); dev = nullptr; };
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

HRESULT RendererD3D::InitD3D(HWND hWnd)
{
#pragma region Swap Chain Description
	//create a struct to hold swap chain info
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount = 1;										// One Back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 32-bit color 
	swapChainDesc.BufferDesc.Width = Window::GetInstance()->GetWidth();			//Set the back buffer width
	swapChainDesc.BufferDesc.Height = Window::GetInstance()->GetHeight();			//Set the back buffer height
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;						//60FPS
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;						//60/1 = 60FPS
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//Intended Swapchain use
	swapChainDesc.OutputWindow = hWnd;										//Windows to use
	swapChainDesc.SampleDesc.Count = 1;										//Number of samples for AA
	swapChainDesc.Windowed = TRUE;										//Windowed/Fullscreen mode
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allow full screen switching

#pragma endregion

#pragma region Create Device and Swap Chain
	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&dev,
		NULL,
		&devCon);

	if (FAILED(hr)) return hr;


	ID3D11Texture2D* pBackBufferTexture = nullptr;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);
	if (FAILED(hr))return hr;

	hr = dev->CreateRenderTargetView(pBackBufferTexture, NULL, &backBuffer);

	pBackBufferTexture->Release();
#pragma endregion

#pragma region Depth Stencil creation

	//Z-Buffer texture description
	D3D11_TEXTURE2D_DESC tex2dDesc = { 0 };
	tex2dDesc.Width = Window::GetInstance()->GetWidth();
	tex2dDesc.Height = Window::GetInstance()->GetHeight();
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;	// SAME COUNT AS SWAP CHAIN
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* zBufferTexture;

	hr = dev->CreateTexture2D(&tex2dDesc, NULL, &zBufferTexture);

	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create Z-Buffer Texture!");
		return E_FAIL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = dev->CreateDepthStencilView(zBufferTexture, &dsvDesc, &zBuffer);

	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil view!");
		return E_FAIL;
	}

	zBufferTexture->Release();


	devCon->OMSetRenderTargets(1, &backBuffer, zBuffer);


	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)Window::GetInstance()->GetWidth();
	viewport.Height = (float)Window::GetInstance()->GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	devCon->RSSetViewports(1, &viewport);


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	dev->CreateSamplerState(&samplerDesc, &sampler);


#pragma region Blend states
	D3D11_BLEND_DESC bd1 = { 0 };
	bd1.RenderTarget[0].BlendEnable = TRUE;
	bd1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd1.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd1.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd1.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd1.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd1.IndependentBlendEnable = FALSE;
	bd1.AlphaToCoverageEnable = FALSE;
	dev->CreateBlendState(&bd1, &pAlphaBlendStateEnable);

	D3D11_BLEND_DESC bd2 = { 0 };
	bd2.RenderTarget[0].BlendEnable = FALSE;
	bd2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd2.IndependentBlendEnable = FALSE;
	bd2.AlphaToCoverageEnable = FALSE;
	dev->CreateBlendState(&bd2, &pAlphaBlendStateDisable);

#pragma endregion

	return S_OK;
}
