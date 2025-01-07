#pragma once
#include <d3d11.h>
#include "Material.h"
#include "objfilemodel.h"
#include "Camera.h"
#include "Mesh.h"

struct CBUFFERSkyBox
{
	DirectX::XMMATRIX WVP;
};
class Skybox
{
private:

	ID3D11RasterizerState* pRasterSolid = NULL;    //Swap between front and back culling
	ID3D11RasterizerState* pRasterSkybox = NULL;//same as above
	ID3D11DepthStencilState* pDepthWriteSolid = NULL;	//Used to disable and enable zBuffer writing
	ID3D11DepthStencilState* pDepthWriteSkybox = NULL;//same as above
	ID3D11Buffer* pSkyboxCBuffer = NULL;		//Cbuffer for the Skybox
	ID3D11SamplerState* pSampler = NULL;

	Mesh* mesh;
	Material* material;
public:
	void DrawSkyBox(ID3D11DeviceContext* devcon, Camera cam);

	void Initialize(ID3D11Device* dev);
};

