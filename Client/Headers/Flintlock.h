#pragma once
#include "MusketObject.h"

#include <random>

namespace Engine
{
	class BitFlag;
	class Animation;
	class Collider;
	class Timer;
}

class CockPin;
class Bubble;
class Rammer;
class Mouse;
class GunPowder;
class Flintlock : public MusketObject
{
public:
	enum Flag : _ullong
	{
		FlagUseAmmunition = 1 << 0,
		FlagMuzzlePowder = 1 << 1,
		FlagFlashHolePowder = 1 << 2,
		FlagGunPowderComplete = 1 << 3,
		FlagRamComplete = 1 << 4,
		FlagAiming = 1 << 5,
		FlagTrigger = 1 << 6,
		FlagMissFire = 1 << 7,
		FlagFire = 1 << 8,
		FlagOverChargePowder = 1 << 9,
		FlagRestarting = 1 << 10,
	};

	enum class FlintlockPivot { Min = -1, Down, Middle, Up, Max };
	enum class PivotDegree { Down = 45, Middle = 15, Up = -5 };
	enum class ReloadAndFireStep { Puor, Spit, Ramming, Aim, Fire, StepEnd };
	enum class RammerStack { Idle = -1, Min, One, Two, Three, Four, Max = 6 };
	enum FlintlockTimer { DelayLoading, DelayFire, TimerEnd };

private:
	explicit Flintlock() = default;
	virtual ~Flintlock() = default;

public:
	int  Update(const float& fDeltaTime) override;
	int  LateUpdate(const float& fDeltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

public:
	void  SettingState(int state) { _currentState = state; }
	int   GetCurrentState() const { return _currentState; }
	void  SettingIsCollisionTrigger(bool isTrigger) { isCollisionTrigger = isTrigger; }
	bool  IsCollisionTrigger() const { return isCollisionTrigger; }
	float GetCurrentDegree() const { return _currentDegree; }
	int	  GetCurrentPivot() const { return _currentPivot; }
	void  SettingRammer(Rammer* pRammer) { _pRammer = pRammer; }
public:
	void  OnUseAmmunition();
	void  SettingMissFire(bool on);
	void  OnFlashHolePowder();
	void  OnOverChargePowder();
	void  OnMuzzlePowder();
	void  OnFire();
	bool  CheckAim();
	bool  CheckTrigger();
	bool  CheckStateFlag(Flag flag);

private:
	bool Initialize(const Vector3& position);
	void Free() override;
	void UpdatePivot(const float& fDeltaTime);
	int  RandomRammerStack();

private:
	void ProcedureRamSlot(Engine::Collider* other);
	void ProcedurePuor(Engine::Collider* other);
	void ProcedureRamming(Engine::Collider* other);
private:
	void UpdatePuor();
	void UpdateSpit();
	void UpdateRamming();
	void UpdateAim();
	void UpdateFireCondition();
	void UpdateTrigger();

public:
	static Flintlock* Create(const Vector3& position);

private:
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Animation* _pAnimation = nullptr;
	Engine::Collider* _pCollider = nullptr;
	Engine::Timer* _pTimer = nullptr;
	CockPin* _pCockPin = nullptr;
	Bubble* _pBubble = nullptr;
	Rammer* _pRammer = nullptr;
	Mouse* _pSelectedMouse = nullptr;
	GunPowder* _pGunPowder = nullptr;
	CollisionInfo _collisionInfo;
	std::random_device rd;

private:
	const float	_rotationSpeed = 85.5f;

private:
	int		_rammerStack = -1;
	int		_currentState = 0;
	int     _oldState = 0;
	int		_currentPivot = 0;
	int     _oldPivot = 0;
	int     _randomInt = 0;
	float	_currentDegree = 15.f;
	float	_targetDegree = 0.f;

private:
	bool isCollisionTrigger = false;
};
