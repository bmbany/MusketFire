#pragma once
#include "GameObject.h"

class FrontGround : public Engine::GameObject
{
private:
	explicit FrontGround() = default;
	virtual ~FrontGround() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void SetIndex(int index) { _index = index; }

private:
	bool Initialize(const wchar_t* textureTag, const Vector3& position);

public:
	static FrontGround* Create(const wchar_t* textureTag, const Vector3& position);

private:
	int _index = 0;
};

