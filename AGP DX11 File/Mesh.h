#pragma once
#include "objfilemodel.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
	XMFLOAT3 Norm;
};
class Mesh
{


private:
	ObjFileModel* fileModel;
	const wchar_t FileLocation;



public:


	void Draw();

};

