#pragma once

#include <d3d11.h>
//This class exists as a container for frequently paired Shader information
class Shader
{
public:
    ID3D11VertexShader* vertexShader = NULL;
    ID3D11PixelShader* pixelShader = NULL;
    ID3D11InputLayout* inputLayout = NULL;

	Shader() : vertexShader(NULL), pixelShader(NULL), inputLayout(NULL)
    {

    }

    Shader(ID3D11VertexShader* VS, ID3D11PixelShader* PS, ID3D11InputLayout* IL)
    {
        vertexShader = VS;
        pixelShader = PS;
        inputLayout = IL;
    }

    ~Shader()
    {
        if (vertexShader) { vertexShader->Release(); vertexShader = nullptr; }
        if (pixelShader) { pixelShader->Release(); pixelShader = nullptr; }
        if (inputLayout) { inputLayout->Release(); inputLayout = nullptr; }
    }

};

