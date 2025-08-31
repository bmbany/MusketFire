#pragma once
#include "Section.h"

class Inventory : public Section
{
private:
	explicit Inventory() = default;
	virtual ~Inventory() = default;

public:
	// Section을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initialize();

public:
	static Inventory* Create();

private:
	std::set<Engine::GameObject*> _inventoryItems;
};