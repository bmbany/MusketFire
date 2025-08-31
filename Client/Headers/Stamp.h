#pragma once
#include "GameObject.h"

class Stamp : public Engine::GameObject
{
private:
	explicit Stamp() = default;
	virtual ~Stamp() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize();

public:
	static Stamp* Create();

};

