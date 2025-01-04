#include "RendererD3D.h"
#include "Window.h"

HRESULT RendererD3D::Initialize(HWND hWnd)
{
	return S_OK;
}
HRESULT RendererD3D::InitD3D(HWND hWnd)
{
#pragma region Swap Chain Description
	//create a struct to hold swap chain info
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount		= 1;										// One Back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 32-bit color 
	swapChainDesc.BufferDesc.Width	= Window::GetInstance()->GetWidth();			//Set the back buffer width
	swapChainDesc.BufferDesc.Height = Window::GetInstance()->GetHeight();			//Set the back buffer height
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;						//60FPS
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;						//60/1 = 60FPS
	swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;			//Intended Swapchain use
	swapChainDesc.OutputWindow		= hWnd;										//Windows to use
	swapChainDesc.SampleDesc.Count	= 1;										//Number of samples for AA
	swapChainDesc.Windowed			= TRUE;										//Windowed/Fullscreen mode
	swapChainDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allow full screen switching

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


	// To be handled elsewhere


	//// Backface culling
	//D3D11_RASTERIZER_DESC rsDesc;
	//ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	//rsDesc.CullMode = D3D11_CULL_BACK;
	//rsDesc.FillMode = D3D11_FILL_SOLID;
	//dev->CreateRasterizerState(&rsDesc, &pRasterSolid);

	////Front Face culling
	//rsDesc.CullMode = D3D11_CULL_FRONT;
	//dev->CreateRasterizerState(&rsDesc, &pRasterSkybox);



	////Depth writing enabled
	//D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSolid);
	//// Depth writing disabled
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSkybox);





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
	viewport.Width = Window::GetInstance()->GetWidth();
	viewport.Height = Window::GetInstance()->GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	devCon->RSSetViewports(1, &viewport);
	//InitPipeline();


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//dev->CreateSamplerState(&samplerDesc, &pSampler);


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