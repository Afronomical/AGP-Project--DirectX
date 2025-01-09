#pragma once





#include <d3d11.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace DirectX;


struct CBUFFERSkybox
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

	//PointLight pointLights[MAX_POINT_LIGHTS];
};
