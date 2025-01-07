#pragma once
#include "objfilemodel.h"
#include "Material.h"

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



public:

	ObjFileModel* fileModel;
	Material* material;
	
	
};

