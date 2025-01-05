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
#pragma region Singleton Pattern

private:
	static Program* instance;
	Program() {};
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;
public:
	static Program* GetInstance() {
		if (!instance) {
			instance = new Program();
		}
		return instance;
	}

#pragma endregion


private:
	
	int Run(HINSTANCE hInstance, int nCmdShow);
	HINSTANCE hInst = NULL;
	HWND	  hWnd = NULL;


public:

	void Initialize(HINSTANCE hInstance, int nCmdShow);
	
	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);



};

