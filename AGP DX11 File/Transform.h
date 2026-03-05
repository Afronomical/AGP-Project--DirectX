#pragma once
#include <DirectXMath.h>


using namespace DirectX;
class Transform
{
public:
	XMFLOAT3 pos{ 0,0,1.5f };
	XMFLOAT3 rot{ 0,0,0.0f };
	XMFLOAT3 scl{ 1,1,1.0f };

	XMMATRIX GetWorldMatrix()
	{
		XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		XMMATRIX scale = XMMatrixScaling(scl.x, scl.y, scl.z);
		XMMATRIX world = scale * rotation * translation;

		return world;
	}



};

