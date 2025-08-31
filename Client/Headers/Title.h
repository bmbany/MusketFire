#pragma once
#include "Scene.h"

namespace Engine
{
	class CollisionManager;
}

class StartButton;
class ExitButton;
class ControlsButton;
class Title : public Engine::Scene
{
private:
	explicit Title() = default;
	virtual ~Title() = default;

public:
	// Scene을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	bool Initialize();

private:
	void Free() override;

public:
	static Title* Create();

private:
	Engine::CollisionManager*	_pCollisionMgr = nullptr;
	StartButton*				_pStartButton = nullptr;
	ExitButton*					_pExitButton = nullptr;
	ControlsButton*             _pControlsButton = nullptr;
};

