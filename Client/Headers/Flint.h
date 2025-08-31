#pragma once
#include "Item.h"

namespace Engine
{
	class Animation;
	class Timer;
}
class CockPin;
class Flintlock;
class Flint : public Item
{
private:
	explicit Flint() = default;
	virtual ~Flint() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initialize(const Vector3& position);
	void Free() override;

public:
	static Flint* Create(const Vector3& position);

private:
	Engine::Animation* _pAnimation = nullptr;
	Engine::Timer* _pTimer = nullptr;
	CockPin* _pCockPin = nullptr;
	Flintlock* _pFlintlock = nullptr;
};