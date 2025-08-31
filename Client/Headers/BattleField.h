#pragma once
#include "Scene.h"

namespace Engine
{
	class CollisionManager;
}

class BattleField : public Engine::Scene
{
private:
	explicit BattleField() = default;
	virtual ~BattleField() = default;

public:
	// Scene��(��) ���� ��ӵ�
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;

	bool Initialize() override;

private:
	void Free() override;

public:
	static BattleField* Create();

private:
	Engine::CollisionManager*	_pCollisionMgr = nullptr;
};

