#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{

public:
	float x = 0, y = 0, z = 0;
	float pitch = XM_PIDIV2, yaw = 100;
	float FOV = 100;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
};

