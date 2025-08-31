#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitmapRenderer;
}
class SacrificeButton : public Engine::GameObject
{
private:
	explicit SacrificeButton() = default;
	virtual ~SacrificeButton() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initialize();

public:
	static SacrificeButton* Create();

private:
	Engine::BitmapRenderer* _pBitmapRederer = nullptr;
	int _index = 0;
	bool _isSacrifice = false;
};

