#pragma once

#include <Windows.h>
#include <string>
class Window
{
private:

	static Window* instance;

	
	 const wchar_t* winName;

	 HINSTANCE hInst;
	 HWND hWnd;

	 int width;
	 int height;

	

	 Window() : hInst(NULL),	hWnd(NULL),
				winName(L"AGP Renderer Window"),
				width(800),		height(600) {};

	 Window(const Window&) = delete;
	 Window& operator=(const Window&) = delete;

public:



	HRESULT Initialize(HINSTANCE instanceHandle, WNDPROC wndProc, int nCmdShow, const wchar_t* name, int width, int height);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND GetWindowHandle();
	int GetWidth();
	int GetHeight();

	static Window* GetInstance() {
		if (!instance) {
			instance = new Window();
		}
		return instance;
	}
	
};

