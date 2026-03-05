#pragma once
#include <d3d11.h>
#include "Material.h"
#include "objfilemodel.h"
#include "Camera.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
class Skybox
{
private:
	ID3D11Device* device = NULL;			//pointer to our direct3d device interface
	ID3D11DeviceContext* devCon = NULL;		//pointer to our Direct 3d device context

	ID3D11RasterizerState* pRasterSolid = NULL;		//Swap between front and back culling
	ID3D11RasterizerState* pRasterSkybox = NULL;	//same as above
	ID3D11DepthStencilState* pDepthWriteSolid = NULL;	//Used to disable and enable zBuffer writing
	ID3D11DepthStencilState* pDepthWriteSkybox = NULL;	//same as above
	ID3D11Buffer* pSkyboxCBuffer = NULL;		//Cbuffer for the Skybox

	Mesh* mesh = nullptr;

	ConstantBuffer<CBUFFERSkybox>* skyboxCBuffer = nullptr; //Cbuffer for the Skybox
	
public:
	void DrawSkyBox();

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* devCon, std::string meshName);

	~Skybox()
	{

		if (pRasterSolid) pRasterSolid->Release(); pRasterSolid = nullptr; 
		if (pRasterSkybox) pRasterSkybox->Release(); pRasterSkybox = nullptr;
		if (pDepthWriteSolid) pDepthWriteSolid->Release(); pDepthWriteSolid = nullptr;
		if (pDepthWriteSkybox) pDepthWriteSkybox->Release(); pDepthWriteSkybox = nullptr;
		if (pSkyboxCBuffer) pSkyboxCBuffer->Release(); pSkyboxCBuffer = nullptr;
		if (skyboxCBuffer) skyboxCBuffer->GetBuffer()->Release(); skyboxCBuffer = nullptr;
		

		if (skyboxCBuffer) delete skyboxCBuffer;


	}
#pragma region Singleton Pattern

private:
	static Skybox* instance;
	Skybox() {};
	Skybox(const Skybox&) = delete;
	Skybox& operator=(const Skybox&) = delete;
public:
	static Skybox* GetInstance() {
		if (!instance) {
			instance = new Skybox();
		}
		return instance;
	}

#pragma endregion
};

