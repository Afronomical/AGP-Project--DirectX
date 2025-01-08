#include "Skybox.h"
#include "Window.h"
#include "MeshManager.h"
#include <iostream>


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
	CBUFFERSkyBox cbuf;
	XMMATRIX translation, projection, view;
	translation = XMMatrixTranslation(g_camera.x, g_camera.y, g_camera.z);
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.1f, 100);
	view = g_camera.GetViewMatrix();
	cbuf.WVP = translation * view * projection;
	devCon->UpdateSubresource(pSkyboxCBuffer, 0, 0, &cbuf, 0, 0);

	//Set Shader resources
	devCon->VSSetConstantBuffers(0, 1, &pSkyboxCBuffer);
	devCon->PSGetSamplers(0, 1, &pSampler);
	devCon->PSSetShaderResources(0, 1, &mesh->material->texture);

	//model->Draw();
	mesh->fileModel->Draw();
	//Backface culling and enable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSolid, 1);
	devCon->RSSetState(pRasterSolid);


	devCon->VSSetShader(mesh->material->shader->vertexShader, 0, 0);
	devCon->PSSetShader(mesh->material->shader->pixelShader, 0, 0);
	devCon->IASetInputLayout(mesh->material->shader->inputLayout);

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

	std::cout << "Skybox::Initialize: Successfully initialized skybox" << endl;
}
