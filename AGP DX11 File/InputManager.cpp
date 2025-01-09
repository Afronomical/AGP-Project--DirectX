
#include "InputManager.h"
#include "Window.h"

void InputManager::Init()
{
	keyboard = std::make_unique<DirectX::Keyboard>();
	mouse = std::make_unique<DirectX::Mouse>();

	mouse->SetWindow(Window::GetInstance()->GetWindowHandle());
	mouse->SetMode(Mouse::MODE_RELATIVE);

	//std::cout << ""
}

void InputManager::HandleInput()
{
	kbState = keyboard->GetState();
	kbTracker->Update(kbState);

	mbState = mouse->GetState();
	msTracker->Update(mbState);

	
}

bool InputManager::GetMBPressed(EMouseBtns btn)
{
	switch (btn)
	{
	case E_LMB:
		return InputManager::GetInstance()->msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED;
		break;
	case E_RMB:
		return InputManager::GetInstance()->msTracker->rightButton == Mouse::ButtonStateTracker::PRESSED;
		break;
	
	case E_MMB:
		return InputManager::GetInstance()->msTracker->middleButton == Mouse::ButtonStateTracker::PRESSED;
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
		return InputManager::GetInstance()->msTracker->leftButton == Mouse::ButtonStateTracker::RELEASED;
		break;
	case E_RMB:
		return InputManager::GetInstance()->msTracker->rightButton == Mouse::ButtonStateTracker::RELEASED;
		break;

	case E_MMB:
		return InputManager::GetInstance()->msTracker->middleButton == Mouse::ButtonStateTracker::RELEASED;
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
		return InputManager::GetInstance()->msTracker->leftButton == Mouse::ButtonStateTracker::HELD;
		break;
	case E_RMB:
		return InputManager::GetInstance()->msTracker->rightButton == Mouse::ButtonStateTracker::HELD;
		break;

	case E_MMB:
		return InputManager::GetInstance()->msTracker->middleButton == Mouse::ButtonStateTracker::HELD;
		break;

	default:
		return false;
		break;
	}
}
