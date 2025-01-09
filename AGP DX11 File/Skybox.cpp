#include "Skybox.h"
#include "Window.h"
#include "MeshManager.h"
#include <iostream>
#include "RendererD3D.h"


Skybox* Skybox::instance = nullptr;
void Skybox::DrawSkyBox(Camera g_camera)
{
	D3D11_CREATE_DEVICE_DEBUG;
	//Front face culling and disable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSkybox, 1);
	devCon->RSSetState(pRasterSkybox);
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

	//Set Skybox shaders
	devCon->VSSetShader(mesh->material->shader->vertexShader, 0, 0);
	devCon->PSSetShader(mesh->material->shader->pixelShader, 0, 0);
	devCon->IASetInputLayout(mesh->material->shader->inputLayout);


	//Constant buffer data
	CBUFFERSkybox cbuf;
	XMMATRIX translation, projection, view;
	translation = XMMatrixTranslation(g_camera.x, g_camera.y, g_camera.z);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.1f, 100);
	view = g_camera.GetViewMatrix();
	cbuf.WVP = translation * view * projection;
	devCon->UpdateSubresource(pSkyboxCBuffer, 0, 0, &cbuf, 0, 0);

	//Set Shader resources
	devCon->VSSetConstantBuffers(0, 1, &pSkyboxCBuffer);
	pSampler = RendererD3D::GetInstance()->GetSampler();
	
	devCon->PSSetShaderResources(0, 1, &mesh->material->texture);
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//model->Draw();
	mesh->fileModel->Draw();
	//Backface culling and enable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSolid, 1);
	devCon->RSSetState(pRasterSolid);

	




	devCon->VSSetShader(mesh->material->shader->vertexShader, 0, 0);
	devCon->PSSetShader(mesh->material->shader->pixelShader, 0, 0);
	devCon->IASetInputLayout(mesh->material->shader->inputLayout);

	devCon->PSSetShaderResources(1, 1, &mesh->material->texture);
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

	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBUFFER0);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	/*if (FAILED(dev->CreateBuffer(&cbd, NULL, &pCBuffer)))
	{
		OutputDebugString(L"Failed to create constant buffer");
		return;
	}*/
	cbd.ByteWidth = sizeof(CBUFFERSkybox);
	if (FAILED(dev->CreateBuffer(&cbd, NULL, &pSkyboxCBuffer)))
	{
		OutputDebugString(L"Failed to  create skybox constant buffer");
		return;
	}

	std::cout << "Skybox::Initialize: Successfully initialized skybox" << endl;
}
