#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include "ReadData.h"
#include <WICTextureLoader.h>
#include "text2D.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "objfilemodel.h"
#include <iostream>
#include <DDSTextureLoader.h>


//#pragma comment (lib, "d3d11.lib")
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define MAX_POINT_LIGHTS 8

using namespace DirectX;

HINSTANCE g_hInst = NULL;
HWND	  g_hWnd  = NULL;
const wchar_t* windowName = L"DirectX Hello World!";

IDXGISwapChain* g_swapchain = NULL; //pointer to swap chain interface
ID3D11Device* g_dev = NULL;			//pointer to our direct3d device interface
ID3D11DeviceContext* g_devcon = NULL; // pointer to our Direct 3d device context
ID3D11RenderTargetView* g_backbuffer = NULL;

FLOAT bgColor[4] = { 0.0f, 0.6f, 1.0f, 1.0f };
FLOAT* bgColors = bgColor;

ID3D11VertexShader* pVS = NULL;// vertex shader
ID3D11PixelShader * pPS = NULL;// pixel shader
ID3D11InputLayout* pLayout = NULL;// the pointer to the input layout

ID3D11Buffer* pVBuffer = NULL; // Vertex Buffer
ID3D11Buffer* pIBuffer = NULL; // Index Buffer
ID3D11Buffer* pCBuffer = NULL; // Constant Buffer
ID3D11DepthStencilView* g_ZBuffer = NULL; // Pointer to Depth Buffer

ID3D11BlendState* pAlphaBlendStateEnable = NULL;
ID3D11BlendState* pAlphaBlendStateDisable = NULL;


ID3D11ShaderResourceView* pTexture = NULL; //Texture
ID3D11SamplerState* pSampler = NULL; 

Keyboard keyboard;
Keyboard::KeyboardStateTracker kbTracker;

Mouse mouse;
Mouse::ButtonStateTracker msTracker;

void HandleInput();


XMVECTOR ambientLightColour = { 0.1f, 0.1f, 0.1f, 1.0f };
XMVECTOR directionalLightShinesFrom = { 0.2788f, 0.7063f, 0.6506f };
XMVECTOR directionalLightColour = { 0.0f, 0.0f, 1.0f, 1.0f };
XMVECTOR directionalLightColour2 = { 1.0f, 0.0f, 0.0f, 1.0f };
XMVECTOR directionalLightColour3 = { 0.0f, 1.0f, 0.0f, 1.0f };


ID3D11RasterizerState* pRasterSolid = NULL;    //Swap between fron and back culling
ID3D11RasterizerState* pRasterSkybox = NULL;//same as above
ID3D11DepthStencilState* pDepthWriteSolid = NULL;	//Used to disable and enable zBuffer writing
ID3D11DepthStencilState* pDepthWriteSkybox = NULL;//same as above
ID3D11Buffer* pSkyboxCBuffer = NULL;		//Cbuffer for the Skybox
ID3D11ShaderResourceView* pSkyboxTexture;	//Skybox texture
ID3D11VertexShader* pVSSkybox = NULL;			//Skybox VS shader
ID3D11PixelShader* pPSSkybox = NULL;		//Skybox PS shader
ID3D11InputLayout* pLayoutSkybox = NULL;	//Input layout for the skybox vertex shader

struct PointLight
{
	XMVECTOR position = { 0,0,0,1 };  // 16 bytes
	XMVECTOR colour = { 1,1,1,1 };	//16 bytes
	
	float strength = 10;		// 4 bytes
	BOOL enabled = false;		// 4 bytes
	float padding[2];			// 8 bytes

	// Total = 48 bytes
};
PointLight pointLights[MAX_POINT_LIGHTS];

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
	XMFLOAT3 Norm;
};

struct CBufferSkybox
{
	XMMATRIX WVP;
};

struct CBUFFER0
{
	XMMATRIX WVP;
	XMMATRIX WV;
	XMVECTOR ambientLightCol;
	XMVECTOR directionalLightDir;
	XMVECTOR directionalLightCol;

	XMVECTOR pointLightPos;
	XMVECTOR pointLightCol;
	float pointLightStrength;
	
	PointLight pointLights[MAX_POINT_LIGHTS];
};
struct Camera
{
	float x = 0, y = 0, z = 0;
	float pitch = XM_PIDIV2, yaw = 0;

	XMMATRIX GetViewMatrix()
	{
		XMVECTOR eyePos{ x, y, z };
		XMVECTOR camUp{ 0, 1, 0 };
		XMVECTOR lookTo{ sin(yaw) * sin(pitch),
						cos(pitch) ,
						cos(yaw) * sin(pitch)
		};
				//world up
		XMMATRIX view = XMMatrixLookToLH(eyePos, lookTo, camUp);
		return view;
	}
};
Camera g_camera;

Text2D* pText;



struct Transform 
{
	XMFLOAT3 pos{ 0,0,1.5f };
	XMFLOAT3 rot{ 0,0,0.0f };
	XMFLOAT3 scl{ 1,1,1.0f };

	XMMATRIX GetWorldMatrix()
	{
		XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		XMMATRIX scale = XMMatrixScaling(scl.x, scl.y, scl.z);
		XMMATRIX world = scale * rotation * translation;

		return world;
	}
};

HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3D(HWND hWnd);
void OpenConsole();
void CleanD3D();
void RenderFrame();

HRESULT InitPipeline();
void InitGraphics();
void InitScene();

Transform cube1;
Transform cube2;
	float speed = 0.001f;

	ObjFileModel* model;
	ObjFileModel* modelSkybox;
	
void DrawSkyBox();



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	
	OpenConsole();
	//Window Creation
	if (FAILED(InitWindow(hInstance, nCmdShow))) 
	{
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	}
	if (FAILED(InitD3D(g_hWnd)))
	{
		MessageBox(NULL, L"Unable to create swapchain and device.", L"Critical Error", MB_ICONERROR | MB_OK);
	}

	//End
	InitGraphics();
	MSG msg;
	InitScene();
	Mouse::Get().SetWindow(g_hWnd);
	Mouse::Get().SetMode(Mouse::MODE_RELATIVE);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{

			HandleInput();
			//Game Code here
			RenderFrame();

		}
	}
	CleanD3D();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:

		case WM_INPUT:
			Keyboard::ProcessMessage(message, wParam, lParam);
			Mouse::ProcessMessage(message, wParam, lParam);
			break;


		case WM_SYSKEYDOWN:
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
			{
				// This is where we handle the "Classic" ALT+ENTER for fullscreen toggle
			}
			Keyboard::ProcessMessage(message, wParam, lParam);
			break;


		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_MOUSEACTIVATE:
			return MA_ACTIVATEANDEAT;

		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::ProcessMessage(message, wParam, lParam);
			break;
		
		default:

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	
}
void HandleInput()
{
	auto kbState = Keyboard::Get().GetState();
	kbTracker.Update(kbState);

	if (kbTracker.lastState.E)
	{
		g_camera.y += speed;
	}
	if (kbTracker.lastState.Q)
	{
		g_camera.y -= speed;
	}

	if (kbState.Escape)
	{
	printf("InputHandling");
		PostQuitMessage(0);
	}
	if (kbState.W)
	{
		g_camera.x += speed * sin(g_camera.yaw) * sin(g_camera.pitch);
		g_camera.y += speed * cos(g_camera.pitch);
		g_camera.z += speed * cos(g_camera.yaw) * sin(g_camera.pitch);
	}
	if(kbState.S)
	{
		g_camera.x -= speed * sin(g_camera.yaw) * sin(g_camera.pitch);
		g_camera.y -= speed * cos(g_camera.pitch);
		g_camera.z -= speed * cos(g_camera.yaw) * sin(g_camera.pitch);
	}
	if (kbState.D)
	{
		g_camera.x += speed * cos(g_camera.yaw);
		g_camera.z += speed * (-sin(g_camera.yaw));
	}
	if (kbState.A)
	{
		g_camera.x -= speed * cos(g_camera.yaw);
		g_camera.z -= speed * (-sin(g_camera.yaw));
	}



	auto msState = Mouse::Get().GetState();
	msTracker.Update(msState);

	float sensitivity = XM_2PI * 0.00025f;
	g_camera.yaw += msState.x * sensitivity;
	g_camera.pitch += msState.y * sensitivity;

	if (msTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		g_camera.x = 0;
		g_camera.y = 0;
		g_camera.z = 0;

		g_camera.pitch = XM_PIDIV2;
		g_camera.yaw = 0;
	}


}
HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow )
{
	g_hInst = instanceHandle;


	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instanceHandle;
	wc.lpszClassName = L"WindowClass1";
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&wc))
	{
		return E_FAIL;
	}
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	
	g_hWnd = CreateWindowEx(NULL,
		L"WindowClass1",
		windowName,
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		instanceHandle,
		NULL);

	

	if (g_hWnd == NULL)return E_FAIL;

	
	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}
HRESULT InitD3D(HWND hWnd)
{
	//create a struct to hold swap chain info
	DXGI_SWAP_CHAIN_DESC scd = {};

	scd.BufferCount = 1;								// One Back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// 32-bit color 
	scd.BufferDesc.Width = SCREEN_WIDTH;				//Set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;				//Set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60;			//60FPS
	scd.BufferDesc.RefreshRate.Denominator = 1;			//60/1 = 60FPS
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//Intended Swapchain use
	scd.OutputWindow = hWnd;							//Windows to use
	scd.SampleDesc.Count = 1;							//Number of samples for AA
	scd.Windowed = TRUE;								//Windowed/Fullscreen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allow full screen switching

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&g_swapchain,
		&g_dev,
		NULL,
		&g_devcon);

	if (FAILED(hr)) return hr;


	// Backface culling
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	g_dev->CreateRasterizerState(&rsDesc, &pRasterSolid);

	//Front Face culling
	rsDesc.CullMode = D3D11_CULL_FRONT;
	g_dev->CreateRasterizerState(&rsDesc, &pRasterSkybox);



	//Depth writing enabled
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	g_dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSolid);
	// Depth writing disabled
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSkybox);





	ID3D11Texture2D* pBackBufferTexture = nullptr;

	hr = g_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);
	if (FAILED(hr))return hr;

	hr = g_dev->CreateRenderTargetView(pBackBufferTexture, NULL, &g_backbuffer);

	pBackBufferTexture->Release();


	//Z-Buffer texture description
	D3D11_TEXTURE2D_DESC tex2dDesc = { 0 };
	tex2dDesc.Width = SCREEN_WIDTH;
	tex2dDesc.Height = SCREEN_HEIGHT;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = scd.SampleDesc.Count;	// SAME COUNT AS SWAP CHAIN
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* zBufferTexture;

	hr = g_dev->CreateTexture2D(&tex2dDesc, NULL, &zBufferTexture);

	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create Z-Buffer Texture!");
		return E_FAIL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = g_dev->CreateDepthStencilView(zBufferTexture, &dsvDesc, &g_ZBuffer);

	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil view!");
		return E_FAIL;
	}

	zBufferTexture->Release();


	g_devcon->OMSetRenderTargets(1, &g_backbuffer, g_ZBuffer);


	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	g_devcon->RSSetViewports(1, &viewport);
	InitPipeline();


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	g_dev->CreateSamplerState(&samplerDesc, &pSampler);



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
	g_dev->CreateBlendState(&bd1, &pAlphaBlendStateEnable);

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
	g_dev->CreateBlendState(&bd2, &pAlphaBlendStateDisable);



	return S_OK;
}
void OpenConsole() {
	
	
	if (AllocConsole())//Console successfully allocated
	{
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);

		std::ios::sync_with_stdio(true);

		std::cout << "Hello Side Console! " << std::endl;
	}
}
void CleanD3D()
{
	if (pRasterSolid)	pRasterSolid->Release();
	if (pDepthWriteSolid) pDepthWriteSolid->Release();
	if (pDepthWriteSkybox) pDepthWriteSkybox->Release();   
	if (pRasterSkybox)	pRasterSkybox->Release();//Skybox
	if (pSkyboxCBuffer)	pSkyboxCBuffer->Release();
	if (pSkyboxTexture)	pSkyboxTexture->Release();
	if (pVSSkybox)		pVSSkybox->Release();
	if (pPSSkybox)		pPSSkybox->Release();
	delete				model;
	delete				modelSkybox;
	if (pAlphaBlendStateDisable) pAlphaBlendStateDisable->Release();
	if (pAlphaBlendStateEnable) pAlphaBlendStateEnable->Release();
	delete				pText;
	if (pTexture)		pTexture->Release();
	if (pSampler)		pSampler->Release();
	if (g_ZBuffer)		g_ZBuffer->Release();
	if (pIBuffer)		pIBuffer->Release();
	if (pCBuffer)		pCBuffer->Release();
	if(pVBuffer)		pVBuffer->Release();
	if (g_backbuffer)	g_backbuffer->Release();
	if (g_swapchain)	g_swapchain->Release();
	if (g_dev)			g_dev->Release();
	if (g_devcon)		g_devcon->Release();
	if (pVS)			pVS->Release();
	if (pPS)			pPS->Release();
	if (pLayout)		pLayout->Release();
	//MessageBox(NULL, L"Thanks for playing!", L"Cleanup Complete", MB_ICONINFORMATION | MB_OK);
}
void RenderFrame()
{
	static double t = 0;
	t += 0.001f;
	double X = sin(t);
	pointLights[1].position = XMVectorSetX(pointLights[1].position, X);

	std::cout << XMVectorGetX(pointLights[1].position)<< ", " << XMVectorGetY(pointLights[1].position)<< ","<< XMVectorGetZ(pointLights[1].position) << endl;
	
	/*static double t = 0;
	t += 0.0001;
	cube1.pos.x = sin(t);
	cube1.rot.y = cos(t) * 0.75;
	cube1.rot.x = sin(t) * 0.75;*/

	g_devcon->ClearRenderTargetView(g_backbuffer, bgColor);
	g_devcon->ClearDepthStencilView(g_ZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DrawSkyBox();


	//select which vertex buffer to use, index buffer and primitive to use PER MESH
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	g_devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	g_devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Select primitive to use
	g_devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_devcon->PSSetSamplers(0, 1, &pSampler);
	g_devcon->PSSetShaderResources(0, 1, &pTexture);
	g_devcon->PSSetShaderResources(1, 1, &pSkyboxTexture);

	CBUFFER0 cbuffer;
	
	XMMATRIX translation, rotation, scale;
	XMMATRIX world, view, projection;




	//VIEW
	view = g_camera.GetViewMatrix();
	
	//PPROJECTION							Vertical FOV		||          Aspect Ratio			||	Clipping planes
	 projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100);
	//XMMATRIX projection = XMMatrixOrthographicLH(XMConvertToRadians(60), SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100);

	//WORLD  PER OBJECT
	
	 //CUBE 1
	 cube1.scl = { 0.5f, 0.5f, 0.5f };
	world = cube1.GetWorldMatrix();
	cbuffer.WVP = world * view * projection;
	cbuffer.WV = world *  view;
	//Lighting
	//AmbientLighting
	//cbuffer.ambientLightCol = ambientLightColour;
	//Directional lighting
	//cbuffer.directionalLightCol = directionalLightColour;
	//XMMATRIX transpose = XMMatrixTranspose(world); //Transpose rotations
	//cbuffer.directionalLightDir = XMVector3Transform(directionalLightShinesFrom, transpose);
	//Point lighting
	for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (!pointLights[i].enabled)
			continue; //Skip disabled lights

		XMMATRIX inverse = XMMatrixInverse(nullptr, world);

		cbuffer.pointLights[i].position = XMVector3Transform(pointLights[i].position, inverse);
		cbuffer.pointLights[i].colour = pointLights[i].colour;
		cbuffer.pointLights[i].strength = pointLights[i].strength;
		cbuffer.pointLights[i].enabled = pointLights[i].enabled;
	}

	g_devcon->UpdateSubresource(pCBuffer, 0, 0, &cbuffer, 0, 0);

	g_devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	//Draw 3 vertices
	//g_devcon->DrawIndexed(36, 0, 0);
	
	model->Draw();
	g_devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	g_devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	////CUBE 2
	XMStoreFloat3(&cube2.pos, pointLights[1].position);
	cube2.scl = { 0.1,0.1,0.1 };

	world = cube2.GetWorldMatrix();
	cbuffer.WVP = world * view * projection;
	////Lighting
	////AmbientLighting
	//cbuffer.ambientLightCol = ambientLightColour;
	////Directional lighting
	//cbuffer.directionalLightCol = directionalLightColour2;
	//
	//cbuffer.directionalLightDir = XMVector3Transform(directionalLightShinesFrom, transpose);
	////Point lighting
	for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (!pointLights[i].enabled)
			continue; //Skip disabled lights

		XMMATRIX inverse = XMMatrixInverse(nullptr, world);

		cbuffer.pointLights[i].position = XMVector3Transform(pointLights[i].position, inverse);
		cbuffer.pointLights[i].colour = pointLights[i].colour;
		cbuffer.pointLights[i].strength = pointLights[i].strength;
		cbuffer.pointLights[i].enabled = pointLights[i].enabled;
	}
	g_devcon->UpdateSubresource(pCBuffer, 0, 0, &cbuffer, 0, 0);

	g_devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	////Draw 3 vertices
	g_devcon->DrawIndexed(36, 0, 0);

	
	

	


	pText->AddText("Die Die Die", -1, +1, 0.075f);

	g_devcon->OMSetBlendState(pAlphaBlendStateEnable, 0, 0xffffffff);
	pText->RenderText();

	g_devcon->OMSetBlendState(pAlphaBlendStateDisable, 0, 0xffffffff);
	g_swapchain->Present(0, 0);
}
void SetRenderColor(float R, float G, float B, float Opacity) 
{
	
	
}

HRESULT LoadVertexShader(LPCWSTR fileName, LPCSTR entryPoint, ID3D11VertexShader** vs, ID3D11InputLayout** i1)
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
	result = g_dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, vs);
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
	result = g_dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), i1);
	VS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	return S_OK;

}
HRESULT LoadPixelShader(LPCWSTR filename, LPCSTR entryPoint, ID3D11PixelShader** ps)
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

	result = g_dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, ps);
	PS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create pixel shader");
		return result;
	}
	return S_OK;
}
HRESULT InitPipeline()
{


	/*LoadVertexShader(L"Compiled Shaders/VertexShader.cso", "main", &pVS, &pLayout);
	LoadPixelShader(L"Compiled Shaders/PixelShader.cso", "main", &pPS);*/




	/*LoadVertexShader(L"VertexShader.hlsl", "main", &pVS, &pLayout);
	LoadPixelShader(L"PixelShader.hlsl", "main", &pPS);*/
	LoadVertexShader(L"ReflectiveVShader.hlsl", "main", &pVS, &pLayout);
	LoadPixelShader(L"ReflectivePShader.hlsl", "main", &pPS);

	LoadVertexShader(L"SkyboxVShader.hlsl", "main", &pVSSkybox, &pLayoutSkybox);
	LoadPixelShader(L"SkyboxPShader.hlsl", "main", &pPSSkybox);

	/*g_devcon->VSSetShader(pVS, 0, 0);
	g_devcon->PSSetShader(pPS, 0, 0);


	g_devcon->IASetInputLayout(pLayout);*/


	return S_OK;
	////Load and compile the vertex and pixel shaders
	//HRESULT result;
	//

	//auto vertexShaderBytecode = DX::ReadData(L"Compiled Shaders/VertexShader.cso");
	//auto pixelShaderBytecode = DX::ReadData(L"Compiled Shaders/PixelShader.cso");


	//g_dev->CreateVertexShader(vertexShaderBytecode.data(), vertexShaderBytecode.size(), NULL, &pVS);
	//g_dev->CreatePixelShader(pixelShaderBytecode.data(), pixelShaderBytecode.size(), NULL, &pPS);

	


	//D3D11_INPUT_ELEMENT_DESC ied[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
	//	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
	//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	//};

	//result = g_dev->CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode.data(), vertexShaderBytecode.size(), &pLayout);

	//ID3D11ShaderReflection* vShaderReflection = NULL;
	//D3DReflect(vertexShaderBytecode.data(), vertexShaderBytecode.size(), IID_ID3D11ShaderReflection, (void**)&vShaderReflection);


	//D3D11_SHADER_DESC desc;
	//vShaderReflection->GetDesc(&desc);

	//D3D11_SIGNATURE_PARAMETER_DESC* signatureParamDescriptions = new D3D11_SIGNATURE_PARAMETER_DESC[desc.InputParameters]{ 0 };
	//for (UINT i = 0; i < desc.InputParameters; i++)
	//{
	//	vShaderReflection->GetInputParameterDesc(i, &signatureParamDescriptions[i]);

	//	ied[i].SemanticName = signatureParamDescriptions[i].SemanticName;
	//	ied[i].SemanticIndex = signatureParamDescriptions[i].SemanticIndex;

	//	if (signatureParamDescriptions[i].ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
	//		switch (signatureParamDescriptions[i].Mask) {
	//		case 1:		ied[i].Format = DXGI_FORMAT_R32_FLOAT;				break;
	//		case 3:		ied[i].Format = DXGI_FORMAT_R32G32_FLOAT;			break;
	//		case 7:		ied[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;		break;
	//		case 15:	ied[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;		break;

	//		default: break;
	//		}
	//		ied[i].InputSlot = 0;
	//		ied[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//		ied[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//		ied[i].InstanceDataStepRate = 0;
	//	}
	//}

	

	/*result = g_dev->CreateInputLayout(ied, desc.InputParameters, vertexShaderBytecode.data(), vertexShaderBytecode.size(), &pLayout);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	g_devcon->IASetInputLayout(pLayout);


	return S_OK;
	delete[]signatureParamDescriptions;
	delete[] ied;*/
}
void InitGraphics()
{

	CreateDDSTextureFromFile(g_dev, g_devcon, L"SkyboxResources/skybox01.dds", NULL, &pSkyboxTexture);
	pText = new Text2D("font1.png", g_dev, g_devcon);
	model = new ObjFileModel{ (char*)"sphere.obj", g_dev, g_devcon };
	modelSkybox = new ObjFileModel{ (char*)"cube.obj", g_dev, g_devcon };
	Vertex vertices[] =
	{
		//			position					colour							Texture UV							NORMAL	
		{XMFLOAT3{-0.5f, -0.5f, -0.5f	}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}	,	XMFLOAT3{-0.5773f, -0.5773f, -0.5773f}	}, //Front BL
		{XMFLOAT3{-0.5f,  0.5f, -0.5f	}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}	,	XMFLOAT3{-0.5773f, 0.5773f, -0.5773f}	},	//Front TL
		{XMFLOAT3{0.5f,  0.5f, -0.5f	}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}	,	XMFLOAT3{0.5773f, 0.5773f, -0.5773f}	},	//Front TR
		{XMFLOAT3{0.5f,  -0.5f, -0.5f	}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}	,	XMFLOAT3{0.5773f, -0.5773f, -0.5773f}	},	//Front BR

		{XMFLOAT3{-0.5f,  -0.5f, 0.5f	}, XMFLOAT4{0.0f, 1.0f, 1.0f, 1.0f}	, XMFLOAT2{0.0f, 1.0f}	,	XMFLOAT3{-0.5773f, -0.5773f, 0.5773f}	},	// Back BL
		{XMFLOAT3{-0.5f,  0.5f, 0.5f	}, XMFLOAT4{1.0f, 0.0f, 1.0f, 1.0f} , XMFLOAT2{0.0f, 0.0f}	,	XMFLOAT3{-0.5773f, 0.5773f, 0.5773f}	},	//Back TL
		{XMFLOAT3{0.5f,  0.5f, 0.5f		}, XMFLOAT4{1.0f, 1.0f, 0.0f, 1.0f} , XMFLOAT2{1.0f, 0.0f}	,	XMFLOAT3{0.5773f, 0.5773f, 0.5773f}		},	//Back TR
		{XMFLOAT3{0.5f,  -0.5f, 0.5f	}, XMFLOAT4{0.0f, 0.0f, 0.0f, 1.0f} , XMFLOAT2{1.0f, 1.0f}	,	XMFLOAT3{0.5773f, -0.5773f, 0.5773f}	},  //Back BR

	};

	unsigned int indices[] = { /*front*/ 0,1,2,2,3,0, /*back*/ 7, 6,5,5,4,7, /*left*/ 4,5,1,1,0,4
							   /*right*/,3,2,6,6,7,3  /*top*/ ,1,5,6,6,2,1,/*bottom*/ 4,0,3,3,7,4 };


	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = indices;

	if (FAILED(g_dev->CreateBuffer(&bufferDesc, &initData, &pIBuffer)))
	{
		OutputDebugString(L"Failed to create index buffer");
		return;
	}

		D3D11_BUFFER_DESC bd = { 0 };
		bd.Usage = D3D11_USAGE_DYNAMIC;
		//bd.ByteWidth = sizeof(Vertex) * 3;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow cpu to write in the buffer
		g_dev->CreateBuffer(&bd, NULL, &pVBuffer);
		
		if (pVBuffer == 0)
		{
			OutputDebugString(L"Failed to create vertex buffer");
			return;
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		g_devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

		memcpy(ms.pData, vertices, sizeof(vertices)); // Copy data to the buffer

		g_devcon->Unmap(pVBuffer, NULL);

		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(CBUFFER0);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		if (FAILED(g_dev->CreateBuffer(&cbd, NULL, &pCBuffer)))
		{
			OutputDebugString(L"Failed to create constant buffer");
			return;
		}
		cbd.ByteWidth = sizeof(CBufferSkybox);
		if (FAILED(g_dev->CreateBuffer(&cbd, NULL, &pSkyboxCBuffer)))
		{
			OutputDebugString(L"Failed to  create skybox constant buffer");
			return;
		}
		CreateWICTextureFromFile(g_dev, g_devcon, L"Box.bmp", NULL, &pTexture);
		
}


void InitScene()
{
	/*cube2.pos = { 0.7f, 0.0f, 3.0 };
	cube2.rot = { 0, XMConvertToRadians(45), 0 };*/

	cube1.pos = { 0.0f, 0.0f, 0.0f };
	cube1.rot = { 0, 0, 0 };
						//position					colour			strength  enabled
	pointLights[0] = { XMVECTOR{1.5f, 1, -1.5f},	{1.0f, 0.0f, 0.0f, 1},  4,	true };
	pointLights[1] = { XMVECTOR{-3.5, -1, -1.0f},	{0.0f, 0.0f, 1.5f, 1},  1,	true };
	pointLights[2] = { XMVECTOR{-1.5, 0, 1.5f},		{0.0f, 1.0f, 0.0f, 1},  4,	true };
	pointLights[3] = { XMVECTOR{1.5f, 0, 1.5},		{1.0f, 0.0f, 1.0f, 1},  4,	true };
}

void DrawSkyBox()
{
	//Front face culling and disable depth write
	g_devcon->OMSetDepthStencilState(pDepthWriteSkybox, 1);
	g_devcon->RSSetState(pRasterSkybox);

	//Set Skybox shaders
	g_devcon->VSSetShader(pVSSkybox, 0, 0);
	g_devcon->PSSetShader(pPSSkybox, 0, 0);
	g_devcon->IASetInputLayout(pLayoutSkybox);


	//Constant buffer data
	CBufferSkybox cbuf;
	XMMATRIX translation, projection, view;
	translation = XMMatrixTranslation(g_camera.x, g_camera.y, g_camera.z);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100);
	view = g_camera.GetViewMatrix();
	cbuf.WVP = translation * view * projection;
	g_devcon->UpdateSubresource(pSkyboxCBuffer, 0, 0, &cbuf, 0, 0);

	//Set Shader resources
	g_devcon->VSSetConstantBuffers(0, 1, &pSkyboxCBuffer);
	g_devcon->PSGetSamplers(0, 1, &pSampler);
	g_devcon->PSSetShaderResources(0, 1, &pSkyboxTexture);

	//model->Draw();
	modelSkybox->Draw();
	//Backface culling and enable depth write
	g_devcon->OMSetDepthStencilState(pDepthWriteSolid, 1);
	g_devcon->RSSetState(pRasterSolid);


	g_devcon->VSSetShader(pVS, 0, 0);
	g_devcon->PSSetShader(pPS, 0, 0);
	g_devcon->IASetInputLayout(pLayout);

}
