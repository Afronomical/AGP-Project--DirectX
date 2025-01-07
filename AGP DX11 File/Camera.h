#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{

public:
	float x = 0, y = 0, z = 0;
	float pitch = XM_PIDIV2, yaw = 0;

	XMMATRIX GetViewMatrix();
	
};

