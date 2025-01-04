#include "Program.h"
#include "Window.h"

void Program::Initialize(HINSTANCE hInstance, int nCmdShow)
{

	//Initialize Game Window
	if (FAILED(Window::GetInstance()->Initialize(hInstance, Window::WindowProc, nCmdShow, L"Somi Renderer Project", 800, 600)))
	{
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	};

	//Initialize Renderer
	/*if (FAILED())
	{

	}*/

}

