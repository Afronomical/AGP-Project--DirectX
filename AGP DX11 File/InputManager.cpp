
#include "Window.h"
#include "InputManager.h"
#include <Keyboard.h>
#include <Mouse.h>

//InputManager* InputManager::instance = nullptr;

HRESULT InputManager::Init()
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();

	kbTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	msTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	mouse->SetWindow(Window::GetInstance()->GetWindowHandle());
	mouse->SetMode(Mouse::MODE_RELATIVE);

	std::cout << "InputManager initialized" << std::endl;
	return S_OK;
}

void InputManager::HandleInput()
{
	kbState = keyboard->GetState();
	mbState = mouse->GetState();

	kbTracker->Update(kbState);
	msTracker->Update(mbState);

}

bool InputManager::GetMBPressed(EMouseBtns btn)
{
	switch (btn)
	{
	case E_LMB:
		return InputManager::GetInstance().msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED;
		break;
	case E_RMB:
		return InputManager::GetInstance().msTracker->rightButton == Mouse::ButtonStateTracker::PRESSED;
		break;
	
	case E_MMB:
		return InputManager::GetInstance().msTracker->middleButton == Mouse::ButtonStateTracker::PRESSED;
		break;

	default:
		return false;
		break;
	}
}
bool InputManager::GetMBReleased(EMouseBtns btn)
{
	switch (btn)
	{
	case E_LMB:
		return InputManager::GetInstance().msTracker->leftButton == Mouse::ButtonStateTracker::RELEASED;
		break;
	case E_RMB:
		return InputManager::GetInstance().msTracker->rightButton == Mouse::ButtonStateTracker::RELEASED;
		break;

	case E_MMB:
		return InputManager::GetInstance().msTracker->middleButton == Mouse::ButtonStateTracker::RELEASED;
		break;

	default:
		return false;
		break;
	}
}
bool InputManager::GetMBHeld(EMouseBtns btn)
{
	switch (btn)
	{
	case E_LMB:
		return InputManager::GetInstance().msTracker->leftButton == Mouse::ButtonStateTracker::HELD;
		break;
	case E_RMB:
		return InputManager::GetInstance().msTracker->rightButton == Mouse::ButtonStateTracker::HELD;
		break;

	case E_MMB:
		return InputManager::GetInstance().msTracker->middleButton == Mouse::ButtonStateTracker::HELD;
		break;

	default:
		return false;
		break;
	}
}
