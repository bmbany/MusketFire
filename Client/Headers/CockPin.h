#pragma once
#include "MusketObject.h"

namespace Engine
{
	class BitFlag;
	class Animation;
	class Collider;
	class Timer;
}
class Bubble;
class Flintlock;
class Section;
class CockPin : public MusketObject
{
public:
	enum Flag : _ullong
	{
		FlagActivate = 1 << 0,
		FlagIdle = 1 << 1,
		FlagCocking = 1 << 2,
		FlagFire = 1 << 3,
		FlagAttached = 1 << 4
	};
	enum class FlintAttach { NotAttached, Attached, End };
	enum class CokPinState { Idle, Cocking, Fire, End };
private:
	explicit CockPin() = default;
	virtual ~CockPin() = default;

public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void UpdateState(const float& deltaTime);
	void UpdatePivot(const float& deltaTime);
	void UpdateFlintAttach();
	void UpdatePosition();
	bool CheckStateFlag(Flag flag);
	void OnStateFlag(Flag flag);
	int  CheckDurablity() const;
	void SetDurablity() { _currentDurablity--; }

private:
	bool Initialize(Engine::GameObject* pFlintlock);
	void Free() override;

public:
	static CockPin* Create(Engine::GameObject* pFlintlock);

private:
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;
	Engine::Collider* _pCollider = nullptr;
	Engine::GameObject* _SelectedMouse = nullptr;
	Engine::Collider* _pPosition = nullptr;
	Flintlock* _pFlintlock = nullptr;
	Section* _pSection = nullptr;
	Bubble* _pBubble = nullptr;

private:
	const int   _flintDurablity = 8;
private:
	int   _currentPivot = 0;
	int   _oldPivot = 0;
	int	  _flintAttach = static_cast<int>(FlintAttach::Attached);
	float _currentDegree = 0.f;
	int   _currentDurablity = _flintDurablity;
	float _targetDegree = 15.f;
	float _calculateDegree[(int)CokPinState::End]{ 15.f, -30.f, 45.f };
};