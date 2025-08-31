#pragma once
#include "GameObject.h"
class ExitButton : public Engine::GameObject
{
private:
	explicit ExitButton() = default;
	virtual ~ExitButton() = default;

public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initialize();

public:
	static ExitButton* Create();

public:
	bool _isExit = false;
private:
	int _index = 0;

};

