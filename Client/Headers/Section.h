#pragma once
#include "GameObject.h"
#include "Mouse.h"

namespace Engine
{
	class BitmapRenderer;
}
class Section : public Engine::GameObject
{
protected:
	explicit Section() = default;
	virtual ~Section() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) = 0;
	int LateUpdate(const float& deltaTime) = 0;
	void Render(Gdiplus::Graphics* pGraphics) = 0;
	void AddRenderer() = 0;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;	

protected:
	Engine::BitmapRenderer* _pBitmapRenderer = nullptr;
	Mouse*					_pMouse = nullptr;	
};

