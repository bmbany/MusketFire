#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitmapRenderer;
}

class BackGround : public Engine::GameObject
{
private:
	explicit BackGround() = default;
	virtual ~BackGround() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize(const wchar_t* textureTag);

public:
	static BackGround* Create(const wchar_t* textureTag);

private:
	Engine::BitmapRenderer* _pBitmamRenderer = nullptr;
	int _renderGroup = 0;
};

