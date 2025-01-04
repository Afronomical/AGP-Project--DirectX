#include "Window.h"
#include <Mouse.h>
#include <Keyboard.h>
#include <d3d11.h>


using namespace DirectX;
Window* Window::instance = nullptr;

HRESULT Window::Initialize(HINSTANCE instanceHandle, WNDPROC wndProc, int nCmdShow, const wchar_t* name, int width, int height)
{
	hInst = instanceHandle;


	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.hInstance = instanceHandle;
	wc.lpszClassName = L"WindowClass1";
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&wc))
	{
		return E_FAIL;
	}
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",
		name,
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		instanceHandle,
		NULL);



	if (hWnd == NULL)return E_FAIL;


	ShowWindow(hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:

	case WM_INPUT:
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		break;


	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// This is where we handle the "Classic" ALT+ENTER for fullscreen toggle
		}
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;


	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_MOUSEACTIVATE:
		return MA_ACTIVATEANDEAT;

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	default:

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}

HWND Window::GetWindowHandle()
{
	return hWnd;
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}
