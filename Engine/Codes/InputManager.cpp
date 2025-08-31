#include "InputManager.h"

using namespace Engine;

void InputManager::Update()
{
    memcpy(&_oldKeyState, &_keyState, sizeof(_keyState));
    memcpy(&_oldMouseState, &_mouseState, sizeof(_mouseState));

    _pKeyBoard->GetDeviceState(sizeof(_keyState), &_keyState);
    _pMouse->GetDeviceState(sizeof(_mouseState), &_mouseState);
}

bool InputManager::IsKeyDown(_byte keycord)
{
    return !(_oldKeyState[keycord] & 0x80) && (_keyState[keycord] & 0x80);
}

bool Engine::InputManager::IsKeyDown(Input::MouseState mouseState)
{
    return !(_oldMouseState.rgbButtons[mouseState] & 0x80) && (_mouseState.rgbButtons[mouseState] & 0x80);
}

bool Engine::InputManager::IsKeyUp(_byte keycord)
{
    return (_oldKeyState[keycord] & 0x80) && !(_keyState[keycord] & 0x80);
}

bool Engine::InputManager::IsKeyUp(Input::MouseState mouseState)
{
    return (_oldMouseState.rgbButtons[mouseState] & 0x80) && !(_mouseState.rgbButtons[mouseState] & 0x80);
}

bool InputManager::IsKeyPress(_byte keycord)
{
    if (_keyState[keycord] & 0x80)
        return true;

    return false;
}

bool Engine::InputManager::IsKeyPress(Input::MouseState mouseState)
{
    if (_mouseState.rgbButtons[mouseState] & 0x80)
        return true;

    return false;
}

bool Engine::InputManager::IsMouseWheel(short keycord)
{
    switch (keycord)
    {
    case -120:
        if (0 > _mouseState.lZ) return true;
        break;
    case 120:
        if (0 < _mouseState.lZ) return true;
        break;
    }    
    
    return false;    
}

InputManager* Engine::InputManager::Create()
{
    return new InputManager;
}

bool Engine::InputManager::Initailize(HINSTANCE hInstance, HWND hWnd)
{
    DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_pInputSDK, NULL);

    _pInputSDK->CreateDevice(GUID_SysKeyboard, &_pKeyBoard, nullptr);
    _pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
    _pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    _pKeyBoard->Acquire();

    _pInputSDK->CreateDevice(GUID_SysMouse, &_pMouse, nullptr);
    _pMouse->SetDataFormat(&c_dfDIMouse);
    _pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    _pMouse->Acquire();

    _pKeyBoard->GetDeviceState(256, _oldKeyState);
    _pMouse->GetDeviceState(sizeof(_oldMouseState), &_oldMouseState);

    return true;
}

void Engine::InputManager::Free()
{
    SafeRelease(_pKeyBoard);
    SafeRelease(_pMouse);
    SafeRelease(_pInputSDK);  
}
