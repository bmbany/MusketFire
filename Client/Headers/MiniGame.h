#pragma once

#include "UI.h"

#include <random>

namespace Engine
{
	class BitFlag;
	class Timer;
}

class MiniGame : public UI
{
public:
	enum Flag : _ullong
	{
		FlagSuccess = 1 << 0,
		FlagFail = 1 << 1,
		FlagOverCharge = 1 << 2,
		FlagActive = 1 << 3,
	};
private:
	explicit MiniGame() = default;
	virtual ~MiniGame() = default;

public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	Vector3 GetGaugePosition() const;
	void UpdateGauge(const float& deltaTime);
	void UpdateFill(const float& deltaTime);
	bool IsSuccess();
	bool IsFail();
	bool IsOverCharge();

private:
	bool Initialize(const float& targetScale);
	void Free() override;
	void CompareGaugeWithTargetBar();
	float RandomHeight();
	void RenderFill(Gdiplus::Graphics* pGraphics);
	void RenderGauge(Gdiplus::Graphics* pGraphics);
	void RenderTargetBar(Gdiplus::Graphics* pGraphics);

public:
	static MiniGame* Create(const float& targetScale);

private:
	Engine::Collider* _pCollider = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;
	std::random_device rd;

private:
	const float _gaugeSpeed = 120.f;
	const float _targetMaxHeight = -10.f;
	const float _targetMinHeight = -40.f;
	const float _gaugeMaxHeight = -170.f;
	const float _gaugeMinHeight = 160.f;

private:
	Vector3 _targetPosition{};
	Vector3 _targetScale{ 1.f, 1.f, 0.f };
	Vector3 _gaugePosition{};
	Vector3 _gaugeCurrentPosition{};

	float _fillScale = 1.f;
	float _topCoordinateY = 0.f;
};
