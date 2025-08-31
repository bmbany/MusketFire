ifdef __MINJAE__
#pragma once
#include "Scene.h"

namespace Engine
{
	class CollisionManager;
	class GameManager;
}

class TestStage : public Engine::Scene
{
public:
	// Scene을(를) 통해 상속됨
	int Update(const float& fDeltaTime) override;
	int LateUpdate(const float& fDeltaTime) override;
	void Free() override;
	bool Initialize();

public:
	static TestStage* Create();

private:
	Engine::CollisionManager*	_pCollisionMgr = nullptr;
	Engine::GameManager*		_pGameMgr = nullptr;
	std::vector<std::string>	_soundKey;
	
	int _index = 0;
	float _pitch = 1.f;

	float _elapsed = 0.f;
	int _fps = 0;
};
#endif