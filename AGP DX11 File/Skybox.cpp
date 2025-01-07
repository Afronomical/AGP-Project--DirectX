#include "Skybox.h"
#include "Window.h"

void Skybox::DrawSkyBox(ID3D11DeviceContext* devCon, Camera g_camera)
{
	//Front face culling and disable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSkybox, 1);
	devCon->RSSetState(pRasterSkybox);

	//Set Skybox shaders
	devCon->VSSetShader(material->GetShader()->vertexShader, 0, 0);
	devCon->PSSetShader(material->GetShader()->pixelShader, 0, 0);
	devCon->IASetInputLayout(material->GetShader()->inputLayout);


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
	devCon->PSSetShaderResources(0, 1, &material->texture);

	//model->Draw();
	mesh->fileModel->Draw();
	//Backface culling and enable depth write
	devCon->OMSetDepthStencilState(pDepthWriteSolid, 1);
	devCon->RSSetState(pRasterSolid);


	devCon->VSSetShader(mesh->material->shader->vertexShader, 0, 0);
	devCon->PSSetShader(mesh->material->shader->pixelShader, 0, 0);
	devCon->IASetInputLayout(mesh->material->shader->inputLayout);

}

void Skybox::Initialize(ID3D11Device* g_dev)
{
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


}
