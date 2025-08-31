#pragma once

#include "Base.h"

namespace Engine
{
	class InputManager : public Base
	{
	private:
		InputManager() = default;
		~InputManager() = default;

	public:
		void Update();

	public:
		bool IsKeyDown(_byte keycord);
		bool IsKeyDown(Input::MouseState mouseState);
		bool IsKeyUp(_byte keycord);
		bool IsKeyUp(Input::MouseState mouseState);
		bool IsKeyPress(_byte keycord);
		bool IsKeyPress(Input::MouseState mouseState);
		bool IsMouseWheel(short keycord);

		bool Initailize(HINSTANCE hInstance, HWND hWnd);
	private:
		void Free() override;

	private:
		LPDIRECTINPUT8			_pInputSDK = nullptr;
		LPDIRECTINPUTDEVICE8	_pKeyBoard = nullptr;
		LPDIRECTINPUTDEVICE8	_pMouse = nullptr;
		_byte					_oldKeyState[256];
		_byte					_keyState[256];

		DIMOUSESTATE			_oldMouseState;
		DIMOUSESTATE			_mouseState;

	public:
		static InputManager* Create();
		
	};
}