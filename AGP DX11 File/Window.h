#pragma once

#include <Windows.h>
#include <string>
class Window
{
private:

	static Window* instance;

	Window() {};
				

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

	int width		= 800;
	int height		= 600;

public:

	HRESULT Initialize(HINSTANCE instanceHandle, WNDPROC wndProc, int nCmdShow, const wchar_t* name, int width, int height);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND GetWindowHandle();
	int GetWidth();
	int GetHeight();

	
	
};

