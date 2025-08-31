#pragma once
#include "Item.h"
namespace Engine
{
	class BitFlag;
	class Animation;
	class Timer;
}
class Mouse;
class Flintlock;
class Rammer : public Item
{
public:
	enum flag : _ullong
	{
		FlagInSlot = 1 << 0,
		FlagRammingPosition = 1 << 1,
		FlagRenderAttached = 1 << 2,
		FlagProcedureInSlot = 1 << 3,
		FlagCompleteRamming = 1 << 4,
		FlagCount = 1 << 5,
	};

	enum class RammerTimer { Disabled, End };
	enum class RenderClip { Idle, Attached };

private:
	explicit Rammer() = default;
	virtual ~Rammer() = default;

public:
	int  Update(const float& fDeltaTime) override;
	int  LateUpdate(const float& fDeltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

public:
	Engine::BitFlag* GetBitFlag() const { return _pBitFlag; }

public:
	void OnStateFlag(flag flag);
	void OffStateFlag(flag flag);
	bool RammerStateFlag(flag flag);
	void RammerLerps(const float& fDeltaTime);
	float GetRammingDegree() const { return _currentDegree; }

private:
	bool Initialize(const Vector3& position, Engine::GameObject* Flintlock);
	void Free() override;
	void UpdatePivot(const float& fDeltaTime);
	void ProcedureRamSlot();
	void CheckStateFlag(const float& fDeltaTime);

public:
	static Rammer* Create(const Vector3& position, Engine::GameObject* Flintlock);

private:
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Animation* _pAnimation = nullptr;
	Engine::Collider* _pCollider = nullptr;
	Mouse* _pSelectedMouse = nullptr;
	Flintlock* _pFlintlock = nullptr;
	Engine::Timer* _pTimer = nullptr;

private:
	const float _gravity = 12.8f;
	const float _maxDegree = 340.f;
	const float _minDegree = 165.f;
	const float _rotationDegree = 360.f;
	const float	_rotationSpeed = 5.f;

private:
	float	_currentDegree = 0.f;
	float	_currentGravity = 0.f;
	int     _currentState = 0;
	Vector3 _rammingStartPosition;
};

