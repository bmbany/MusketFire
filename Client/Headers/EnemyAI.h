#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitFlag;
	class Timer;
}

class Bubble;
class Army;
class Soldier;
class Canon;
class EnemyAI : public Engine::GameObject
{
private:
	enum Timer { FSM, Timer_End };
	struct AIInfo
	{
		AIInfo(Soldier* pSoldier, float delay, float startDelay)
			: pSoldier(pSoldier), delay(delay), startDelay(startDelay) {}
		AIInfo(Canon* pCanon, float delay, float startDelay)
			: pCanon(pCanon), delay(delay), startDelay(startDelay) {}
		Soldier* pSoldier = nullptr;
		Canon* pCanon = nullptr;
		float delay = 0.f;
		float elapsed = 0.f;
		float startDelay = 0.f;
		bool isFirst = true;
	};
private:
	explicit EnemyAI() = default;
	virtual ~EnemyAI() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

	void AddEnemy(int row, Soldier* pSoldier, float delay, float startDelay);
	void AddEnemy(int row, Canon* pCanon, float delay, float startDelay);
	void DeleteEnemy(int row, Army* pArmy);
	void DeleteAllEnemy();
	int GetAttackRow() { return _attackRow; }
	int GetAttackCount() { return _attackCount; }
	bool IsAttackCanon() { return _isAttackCanon; }
	void SetEnemyStartCount() { _startEnemyCount = (float)_alliveEnemy; }

private:
	// Base을(를) 통해 상속됨
	bool Initialize();
	void Free() override;

public:
	static EnemyAI* Create();

private:
	std::vector<AIInfo> _enemys[3];
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;
	Bubble* _pBubble = nullptr;
	int _attackRow = -1;
	int _attackCount = -1;
	int _alliveEnemy = 0;
	float _startEnemyCount = 0.f;
	bool _isAttackCanon = false;
	bool _isDefeat = false;
};

