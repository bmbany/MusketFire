#pragma once
#include "Item.h"

namespace Engine
{
	class Animation;
	class Collider;
}
class Mouse;
class Flintlock;
class Ammunition : public Item
{
private:
	explicit Ammunition() = default;
	virtual ~Ammunition() = default;

public:
	int Update(const float& fDeltaTime) override;
	int LateUpdate(const float& fDeltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
private:
	bool Initialize(const Vector3& position);
	void Free() override;

public:
	static Ammunition* Create(const Vector3& position);

private:
	Engine::Collider* _pCollider = nullptr;
	Flintlock* _pFlintlock = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
};

