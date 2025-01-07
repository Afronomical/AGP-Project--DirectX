#pragma once

#include <d3d11.h>
#include "Shader.h"
#include <string>

class Material
{
private:
	

public:
	ID3D11ShaderResourceView* texture;	// Texture or image to overlay
	Shader* shader;						//Container for Vertex and Pixel Shader
	Material() {};
	Material(LPCWSTR textureFileLocation, std::string shaderName);

	ID3D11ShaderResourceView* GetTexture() { return texture; };
	Shader* GetShader() { return shader; };
};

