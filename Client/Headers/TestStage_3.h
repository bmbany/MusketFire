#ifdef __GANGJIN__

#pragma once
#include "Scene.h"

namespace Engine
{
	class CollisionManager;
}

class CombatManager;
class TestStage : public Engine::Scene
{
	// Scene을(를) 통해 상속됨
	int Update(const float& fDeltaTime) override;
	int LateUpdate(const float& fDeltaTime) override;
	void Free() override;
	bool Initialize();

public:
	static TestStage* Create();

private:
	Engine::CollisionManager*	_pCollisionMgr = nullptr;
	CombatManager*				_pCombatMgr = nullptr;

	float _elapsed = 0.f;
	int _fps = 0;
};

#endif