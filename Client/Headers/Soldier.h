#pragma once
#include "Army.h"

namespace Engine
{
	class Animation;
	class Timer;
	class BitFlag;
}

class Soldier : public Army
{
	friend class EnemyAI;
public:
	enum Animation
	{
		Idle, Walk, AimStart, Fire, Death, ChargeWalk, Stab,
		Animation_End
	};
	enum State
	{
		State_Idle, State_Walk, State_Aim, State_Fire, State_Death, State_Charge
	};
	enum Type { Player, Enemy, SpawnSoldier };
private:
	enum Timer { SceneChange, Smoke, WalkSound, Timer_End };
	enum Flag : _ullong
	{
		FlagFire = 1 << 1,
		FlagMove = 1 << 2,
		FlagDeath = 1 << 3,
		FlagCharge = 1 << 4,
		FlagStart = 1 << 5
	};

private:
	explicit Soldier() = default;
	virtual ~Soldier() = default;

public:
	// Army을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void ChangeAnimation(Animation animation);
	void ChargeAttack();
	void OnCollisionEnter(CollisionInfo info) override;
	void OnCollision(CollisionInfo info) override;
	void SetStateDeath() override;
	void SetStartPosition();
	bool IsChargetAttack();

	Animation GetCurrAnimation() { return _currAnimation; }

private:
	void StateIdle();
	void StateWalk();
	void StateAim();
	void StateFire();
	void StateDeath();
	void StateCharge();

private:
	bool Initialize(const Vector3& position, Type type, bool isCharge);

public:
	static Soldier* Create(const Vector3& position, Type type, bool isCharge = false);

private:
	Vector3				_startPosition;
	Animation			_currAnimation = Animation_End;
	State				_currState;
	Type				_type;
	float				_deltaTime = 0.f;
	float				_speed = 0.f;
	bool				_isChargeInfantry = false;
};

