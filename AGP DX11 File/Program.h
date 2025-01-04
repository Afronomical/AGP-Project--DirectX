#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include "ReadData.h"
#include <WICTextureLoader.h>
#include "text2D.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "objfilemodel.h"
#include <iostream>
#include <DDSTextureLoader.h>
using namespace DirectX;

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define MAX_POINT_LIGHTS 8

class Program
{

	int Run();
	HINSTANCE hInst = NULL;
	HWND	  hWnd = NULL;


private:


	void Initialize(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow);
	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);



};

