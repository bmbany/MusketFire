#pragma once
#include "Base.h"

namespace Engine
{
	class GameManager;
}
class MainGame : public Engine::Base
{
private:
	explicit MainGame() = default;
	virtual ~MainGame() = default;
	NOCOPY(MainGame)

public:
	bool Run();

private:
	bool Initialize(HINSTANCE hInstance, HWND hWnd, HDC hdc);
	void Free() override;

public:
	static MainGame* Create(HINSTANCE hInstance, HWND hWnd, HDC hdc);

private:
	Engine::GameManager* _pGameManager = nullptr;
};

