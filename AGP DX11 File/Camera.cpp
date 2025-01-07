#include "Camera.h"

XMMATRIX Camera::GetViewMatrix()
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
