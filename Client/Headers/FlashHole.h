#pragma once
#include "MusketObject.h"

namespace Engine
{
	class BitFlag;
	class Animation;
	class Collider;
}
class PaperCartidge;
class GunPowder;
class Mouse;
class Flintlock;
class FlashHole : public MusketObject
{
public:
	enum Flag : _ullong
	{
		FlagOpen = 1 << 0,
		FlagUseGunPowder = 1 << 1,
		FlagGunPowder = 1 << 2,
		FlagPaperCartidge = 1 << 3,
	};

	enum class FlashHoleState { Open, Closed, End };
private:
	explicit FlashHole() = default;
	virtual ~FlashHole() = default;
public:
	int  Update(const float& fDeltaTime) override;
	int  LateUpdate(const float& fDeltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void UpdatePosition();
	void UpdateState();
	void SetStateFlag(Flag flag);
	void UpdateUseGunPowder();
	void UpdateUseGunPowderPaperCartidge();

public:
	bool Initalize(Engine::GameObject* pFlintlock);
	void Free() override;
public:
	static FlashHole* Create(Engine::GameObject* pFlintlock);
private:
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Collider* _pCollider = nullptr;
	Engine::Collider* _pPosition = nullptr;

	Mouse* _pSelectedMouse = nullptr;
	Flintlock* _pFlintlock = nullptr;
	PaperCartidge* _pPaperCartidge = nullptr;
	GunPowder* _pGunPowder = nullptr;

private:
	float _currentDegree = 0.f;
	int _currentState = 0;
};