#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
class Window
{
private:

	static Window* instance;

	Window() {};
	Window(const wchar_t* windowName) : winName(windowName) {};
				

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

public:
	static Window* GetInstance() {
		if (!instance) {
			instance = new Window();
		}
		return instance;
	}
private:

	const wchar_t* winName = (L"AGP Renderer Window");

	HINSTANCE hInst	= NULL;
	HWND hWnd		= NULL;

	int width		= 1920;
	int height		= 1080;

public:

	HRESULT Initialize(HINSTANCE instanceHandle, WNDPROC wndProc, int nCmdShow, const wchar_t* name, int width, int height);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND GetWindowHandle();
	int GetWidth();
	int GetHeight();
	void Clean()
	{
		if (hWnd) 
		{
			DestroyWindow(hWnd);
			hWnd = NULL;
		}
		if (hInst) 
		{
			UnregisterClass(winName, hInst);
			hInst = NULL;
		}
		if (instance) 
		{
			delete instance;
			instance = nullptr;
		}
		if (winName)
		{
			winName = nullptr;
		}
		std::cout << "Window cleaned up" << std::endl;

		
		
	}
	
	
};

