#include <cstdlib>
#include "WinApp.h"
#include "MainGame.h"
#include "GameManager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

constexpr int SCREEN_START_LEFT = 10;
constexpr int SCREEN_START_TOP = 10;
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif
#ifdef NDEBUG
DEVMODE originalDevMode;

void SaveCurrentResolution() {
	ZeroMemory(&originalDevMode, sizeof(originalDevMode));
	originalDevMode.dmSize = sizeof(originalDevMode);
	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &originalDevMode)) {
		std::cerr << "Failed to get current resolution" << std::endl;
	}
}

void ChangeResolution(int width, int height) {
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);
	devMode.dmPelsWidth = width;
	devMode.dmPelsHeight = height;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	LONG result = ChangeDisplaySettings(&devMode, 0);
	if (result != DISP_CHANGE_SUCCESSFUL) {
		std::cerr << "Failed to change resolution" << std::endl;
	}
}
#endif // NDEBUG


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WinApp winApp;

	winApp.Initialize(hInstance);
	winApp.Run();
	winApp.Finalize();

	_CrtDumpMemoryLeaks();

	return EXIT_SUCCESS;
}

LRESULT CALLBACK WinApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void WinApp::Initialize(HINSTANCE hInstance)
{
	_hInstance = hInstance;

	const TCHAR* appName = TEXT("MusketFire");

	//Step 1: Registering the Window Class

	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = appName;

	RegisterClass(&wndClass);

	// Step 2: Creating the Window
#ifdef NDEBUG

	SaveCurrentResolution();

	ChangeResolution(1280, 720);

	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
			SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	_hWnd = CreateWindow(appName, appName, WS_POPUP | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(_hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(_hWnd);

	_hDC = GetDC(_hWnd);

	// Step 3: Game Initialize Here
	_pMainGame = MainGame::Create(_hInstance, _hWnd, _hDC);
	_pGameManager = Engine::GameManager::GetInstance();
#endif

#ifdef _DEBUG
	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
	SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	_hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(_hWnd, SW_NORMAL);
	UpdateWindow(_hWnd);

	_hDC = GetDC(_hWnd);

	// Step 3: Game Initialize Here
	_pMainGame = MainGame::Create(_hInstance, _hWnd, _hDC);
	_pGameManager = Engine::GameManager::GetInstance();
#endif
}

void WinApp::Run()
{
	// Step 4: Game Loop Here
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;
			else
			{
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!_pMainGame->Run())
				break;
		}
	}
}

void WinApp::Finalize()
{
	// Step 5: Game Finalize Here
	ReleaseDC(_hWnd, _hDC);
	Engine::SafeRelease(_pMainGame);
#ifdef NDEBUG
	ChangeResolution(originalDevMode.dmPelsWidth, originalDevMode.dmPelsHeight);
#endif // NDEBUG
}
