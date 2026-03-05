#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "PointLight.h"

using namespace DirectX;

#define MAX_POINT_LIGHTS 8

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

	PointLight pointLights[MAX_POINT_LIGHTS];
};
struct WaveCBuffer {
	XMMATRIX W;
	XMMATRIX V;
	XMMATRIX P;
	float time;
	float padding[3]; // Padding for 16-byte alignment
};