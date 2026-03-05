#include "Program.h"
#include "Window.h"
#include "RendererD3D.h"
#include  "InputManager.h"
#include "DeltaTime.h"


Program* Program::instance = nullptr;
int Program::Run()
{



	MSG msg;
	while (isRunning)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			DeltaTime::UpdateTime();
			//HandleInput();
			InputManager::GetInstance().HandleInput();
			//SceneUpdate

			//Game Code here
			currentScene->Play();
			//RenderFrame();
			RendererD3D::GetInstance()->RenderFrame(currentScene);

			if (InputManager::GetInstance().GetKeyPressed().Escape)
			{
				Clean();
				isRunning = false;
			}


		}
	}

	return 0;
}

void Program::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (AllocConsole())//Console successfully allocated
	{
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);

		std::ios::sync_with_stdio(true);

		std::cout << "Hello Side Console! " << std::endl;
	}
	//Initialize Game Window
	if (FAILED(Window::GetInstance()->Initialize(hInstance, Window::WindowProc, nCmdShow, L"Somi Renderer Project", Window::GetInstance()->GetWidth()/2, Window::GetInstance()->GetHeight()/2)))
	{
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	};


	//Initialize Renderer
	if (FAILED(RendererD3D::GetInstance()->Initialize(Window::GetInstance()->GetWindowHandle(), currentScene)))
	{
		MessageBox(NULL, L"Unable to create swapchain and device.", L"Critical Error", MB_ICONERROR | MB_OK);
	}

	//Initialize InputManager
	if (FAILED(InputManager::GetInstance().Init()))
	{
		MessageBox(NULL, L"Failed to initialise input", L"Critical Error", MB_ICONERROR | MB_OK);
	}

}




void Program::Clean()
{
	RendererD3D::GetInstance()->Clean();
	currentScene->CleanScene();
	Window::GetInstance()->Clean();

}

