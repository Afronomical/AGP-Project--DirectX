#include "Program.h"
#include <IOstream>


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	
	Program::GetInstance()->Initialize(hInstance, nCmdShow);
	Program::GetInstance()->Run();

	std :: cout << "Exiting Program" << std::endl;
	std::cin.get();
	
	return 0;
}


