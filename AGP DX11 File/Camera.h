#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{


	float x = 0, y = 0, z = 0;
	float pitch = XM_PIDIV2, yaw = 0;

	XMMATRIX GetViewMatrix()
	{
		XMVECTOR eyePos{ x, y, z };
		XMVECTOR camUp{ 0, 1, 0 };
		XMVECTOR lookTo{ sin(yaw) * sin(pitch),
						cos(pitch) ,
						cos(yaw) * sin(pitch)
		};
		//world up
		XMMATRIX view = XMMatrixLookToLH(eyePos, lookTo, camUp);
		return view;
	}
};

