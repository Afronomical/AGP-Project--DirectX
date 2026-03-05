#include "Skybox.h"
#include "Window.h"
#include "MeshManager.h"
#include <iostream>
#include "RendererD3D.h"



Skybox* Skybox::instance = nullptr;
void Skybox::DrawSkyBox()
{
	if (!mesh) {
		std::cerr << "Error: mesh is null!" << std::endl;
	}
	else if (!mesh->material) {
		std::cerr << "Error: material is null!" << std::endl;
	}
	else if (!mesh->material->shader) {
		std::cerr << "Error: shader is null!" << std::endl;
	}
	else if (!mesh->material->shader->pixelShader) {
		std::cerr << "Error: pixelShader is null!" << std::endl;
	}
	//Front face culling and disable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSkybox, 1);
	devCon->RSSetState(pRasterSkybox);
	//Constant buffer data
	Camera* g_camera = RendererD3D::GetInstance()->cam;

	CBUFFERSkybox cbuf;
	XMMATRIX translation, projection, view;
	translation = XMMatrixTranslation(g_camera->x, g_camera->y, g_camera->z);
	projection = g_camera->GetProjectionMatrix();
	view = g_camera->GetViewMatrix();
	cbuf.WVP = translation * view * projection;

	//devCon->UpdateSubresource(pSkyboxCBuffer, 0, nullptr, &cbuf, 0, 0);
	skyboxCBuffer->Update(devCon, cbuf);
	
	RendererD3D::GetInstance()->DrawMesh(mesh, pSkyboxCBuffer, &cbuf, sizeof(CBUFFERSkybox));
	

	devCon->OMSetDepthStencilState(pDepthWriteSolid, 1);
	devCon->RSSetState(pRasterSolid);

	
}

void Skybox::Initialize(ID3D11Device* dev, ID3D11DeviceContext* devContext, std::string MeshName)
{
	device = dev;
	devCon = devContext;


	mesh = MeshManager::GetInstance()->GetMesh(MeshName);
	

	// Backface culling
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	dev->CreateRasterizerState(&rsDesc, &pRasterSolid);

	//Front Face culling
	rsDesc.CullMode = D3D11_CULL_FRONT;
	dev->CreateRasterizerState(&rsDesc, &pRasterSkybox);


	//Depth writing enabled
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSolid);
	// Depth writing disabled
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dev->CreateDepthStencilState(&dsDesc, &pDepthWriteSkybox);

	skyboxCBuffer = new ConstantBuffer<CBUFFERSkybox>(device);

	std::cout << "Skybox::Initialize: Successfully initialized skybox" << endl;
}
