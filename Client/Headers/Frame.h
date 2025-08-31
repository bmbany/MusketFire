#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitmapRenderer;
}
class Frame : public Engine::GameObject
{
private:
	explicit Frame() = default;
	virtual ~Frame() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize();

public:
	static Frame* Create();

private:
	Engine::BitmapRenderer* _pBitmapRenderer = nullptr;
};

