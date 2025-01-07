#include "Program.h"



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	
	Program::GetInstance()->Initialize(hInstance, nCmdShow);
	Program::GetInstance()->Run();
	return 0;
}


