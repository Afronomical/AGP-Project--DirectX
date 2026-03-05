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
#include "Scene.h"
#include "SceneDemo.h"
#include "SpaceScene.h"
using namespace DirectX;

class Program
{
#pragma region Singleton Pattern

private:
	static Program* instance;
	Program() {};
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;

	~Program()
	{
		if (currentScene) delete currentScene;
		if (instance) delete instance;
	}


public:
	static Program* GetInstance() {
		if (!instance) {
			instance = new Program();
		}
		return instance;
	}

#pragma endregion


private:

	HINSTANCE hInst = NULL;
	HWND	  hWnd = NULL;
	bool isRunning = true;
	Scene* currentScene = new SpaceScene();
						//new SceneDemo();


public:

	int Run();
	void Initialize(HINSTANCE hInstance, int nCmdShow);
	void Clean();



};

