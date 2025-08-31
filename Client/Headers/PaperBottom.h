#pragma once

#include "item.h"

class PaperCartidge;
class PaperBottom : public Item
{
public:
	enum Flag : _ullong
	{
		FlagSplit = 1 << 0,
	};
private:
	explicit PaperBottom() = default;
	virtual ~PaperBottom() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	bool IsSplit() const;
	void UpdateCartidgeDead();

private:
	void UpdatePosition(const float& deltaTime);

private:
	bool Initialize(PaperCartidge* pPaperCartidge);
	void Free() override;

public:
	static PaperBottom* Create(PaperCartidge* pPaperCartidge);

private:
	Engine::Collider* _pCollider = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;
	PaperCartidge* _pPaperCartidge = nullptr;

};