#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include <iostream>
#include <d3d11.h>
#include <Vector>


using namespace DirectX;


enum EMouseBtns
{
    E_LMB,
    E_RMB,
    E_MMB,
    E_ScrollUp,
    E_ScrollDown,
};
class InputManager
{
private:
    DirectX::Keyboard::State kbState;
    std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> kbTracker;
    std::unique_ptr<DirectX::Keyboard> keyboard;
    
    DirectX::Mouse::State mbState;
    std::unique_ptr<DirectX::Mouse::ButtonStateTracker> msTracker;
    std::unique_ptr<DirectX::Mouse> mouse;

public:
	HRESULT Init();
	void HandleInput();

private:




    
    InputManager() {};
	~InputManager() {
		if (kbTracker) kbTracker.reset();
		if (keyboard) keyboard.reset();
		if (msTracker) msTracker.reset();
		if (mouse) mouse.reset();
	}
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;



public:



    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    // E.G: GetKeyPressed().H
    static DirectX::Keyboard::State GetKeyPressed() { return InputManager::GetInstance().kbTracker->pressed;}
    // E.G: GetKeyReleased().E
    static DirectX::Keyboard::State GetKeyReleased() { return InputManager::GetInstance().kbTracker->released;}
    // E.G: GetKeyHeld().L
    static DirectX::Keyboard::State GetKeyHeld() { return InputManager::GetInstance().kbTracker->lastState;}
    // E.G: GetMouse().X
    static DirectX::Mouse::State GetMouseXY() { return InputManager::GetInstance().mbState; }


    // Parameters: 0=LeftMB, 1=RightMB, 2=MiddleMB
    // //
    // E.G:  GetMBPressed(1)     or      GetMBPressed(EMouseBtns::E_RMB)
    bool GetMBPressed(EMouseBtns);
    // Parameters: 0=LeftMB, 1=RightMB, 2=MiddleMB
    // //
    // E.G:  GetMBReleased(1)     or      GetMBReleased(EMouseBtns::E_RMB)
    bool GetMBReleased(EMouseBtns);
    // Parameters: 0=LeftMB, 1=RightMB, 2=MiddleMB
    // //
    // E.G:  GetMBHeld(1)     or      GetMBHeld(EMouseBtns::E_RMB)
    bool GetMBHeld(EMouseBtns buttonIndex);
    
 



};

