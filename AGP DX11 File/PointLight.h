#pragma once

#include <directxmath.h>
#include <Windows.h>

class PointLight
{
public:
	DirectX::XMVECTOR position = { 0.0f, 1.0f, -1.0f }; //16 bytes
	DirectX::XMVECTOR colour = { 0.55f, 1.0f, 0.65f, 1.0f }; //16 bytes
	float strength = 10.0f;		//4 bytes
	BOOL active = false;		//4 bytes
	float padding[2];			//8 bytes

	//Total = 48
};

