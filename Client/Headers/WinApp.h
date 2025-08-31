/**

	@file      WinApp.h
	@brief     Let's create a window and wrap the message procedure in a class.
	@author    SKYFISH
**/
#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>

namespace Engine
{
	class GameManager;
}
class MainGame;
class WinApp
{
public:
	WinApp() = default;
	~WinApp() = default;

	void Initialize(HINSTANCE hInstance);

	void Run();

	void Finalize();

	HINSTANCE GetInstance() const { return _hInstance; }
	HWND GetWindow() const { return _hWnd; }

	const int GetWidth() const { return _width; }
	const int GetHeight() const { return _height; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE				_hInstance = { 0 };  // HINSTANCE is a handle to an instance of a module.
	HWND					_hWnd = { 0 };			// HWND is a handle to a window.
	HDC						_hDC = { 0 };
	MainGame*				_pMainGame = nullptr;
	Engine::GameManager*	_pGameManager = nullptr;
	int						_width = 1024;
	int						_height = 768;
};