#pragma once
#include <d3d11.h>
#include "Material.h"
#include "objfilemodel.h"
#include "Camera.h"
#include "Mesh.h"
#include "Buffers.h"
class Skybox
{
private:
	ID3D11Device* device = NULL;			//pointer to our direct3d device interface
	ID3D11DeviceContext* devCon = NULL; // pointer to our Direct 3d device context

	ID3D11RasterizerState* pRasterSolid = NULL;    //Swap between front and back culling
	ID3D11RasterizerState* pRasterSkybox = NULL;//same as above
	ID3D11DepthStencilState* pDepthWriteSolid = NULL;	//Used to disable and enable zBuffer writing
	ID3D11DepthStencilState* pDepthWriteSkybox = NULL;//same as above
	ID3D11Buffer* pSkyboxCBuffer = NULL;		//Cbuffer for the Skybox
	ID3D11SamplerState* pSampler = NULL;

	Mesh* mesh;
	
public:
	void DrawSkyBox( Camera cam);

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* devCon, std::string meshName);

	ID3D11RasterizerState* GetRasterSolid()
	{ 
		 
		return pRasterSolid;
	};
	ID3D11RasterizerState* GetRasterSkybox()
	{

		return pRasterSkybox;
	};

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

