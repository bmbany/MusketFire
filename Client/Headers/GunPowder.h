//#pragma once
#include "Item.h"

namespace Engine
{
	class Timer;
}
class Effect;
class FlashHole;
class Flintlock;
class MiniGame;
class GunPowder : public Item
{
public:
	enum Flag : _ullong
	{
		FlagSuccess = 1 << 0,
		FlagFail = 1 << 1,
		FlagOverCharge = 1 << 2,
		FlagCorrectUse = 1 << 3,
		FlagAnimation = 1 << 4,
	};

	enum Timer { Effect, Sound, End };
private:
	explicit GunPowder() = default;
	virtual ~GunPowder() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void StateCharge();
	bool IsSuccess() const;
	bool IsFail() const;
	bool IsOverCharge() const;

private:
	bool Initialize(const Vector3& position);
	void Free() override;
private:
	void DrawString(Gdiplus::Graphics* pGraphics,
		Gdiplus::Color color, float x, float y,
		const WCHAR* string);
	void CreateMiniGame(const float& deltaTime);
	void UpdateMiniGame();

public:
	static GunPowder* Create(const Vector3& position);

private:
	Engine::Timer* _pTimer = nullptr;
	FlashHole* _pFlashHole = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	MiniGame* _pMiniGame = nullptr;
	Flintlock* _pFlintlock = nullptr;
	Gdiplus::Rect _drawRect{};

private:
	const int _maxStack = 16;
	const int _maxFrame = 6;
private:
	Vector3 _animationOffset{};
	int _currentStack = _maxStack;
	float _currentGaugeScale = 0.f;
	float _currentFrame = 0.f;
	int _currentFrameIndex = 0;
	bool _isLastFrame = false;
};