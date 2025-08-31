#pragma once
#include "GameObject.h"
class StartButton : public Engine::GameObject
{
private:
	explicit StartButton() = default;
	virtual ~StartButton() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	bool IsSceneChange() { return _isSceneChange; }

private:
	bool Initialize();

public:
	static StartButton* Create();

private:
	bool _isSceneChange;
	int _index = 0;
};

