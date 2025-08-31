#pragma once
#include "Item.h"

class PlayerStatus;
class Biscuit : public Item
{
public:
	enum BiscuitFlag : _ullong
	{
		FlagEat = 1 << 0,
	};

	enum class BiscuitTimer { Bite, Eat, End };
private:
	explicit Biscuit() = default;
	virtual ~Biscuit() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	bool CheckEat();

private:
	bool Initialize(const Vector3& position, PlayerStatus* player);
	void Free() override;

public:
	static Biscuit* Create(const Vector3& position, PlayerStatus* player);

private:
	Engine::Collider* _pCollider = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;
	PlayerStatus* _pPlayer = nullptr;
};