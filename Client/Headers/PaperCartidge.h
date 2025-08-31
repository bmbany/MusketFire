#pragma once
#include "Item.h"

class Flintlock;
class PaperCartidge : public Item
{
public:
	enum Flag : _ullong
	{
		FlagOnSplit = 1 << 0,
		FlagUseable = 1 << 1,
	};
private:
	explicit PaperCartidge() = default;
	virtual ~PaperCartidge() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void SetUseable();
	bool IsOnSplit() const;
	bool IsUseable() const;

private:
	bool Initialize(const Vector3& position);
	void Free() override;

public:
	static PaperCartidge* Create(const Vector3& position);

private:
	Engine::Collider* _pCollider = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Flintlock* _pFlintlock = nullptr;

};