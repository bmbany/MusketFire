#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitFlag;
}

class ControlsButton : public Engine::GameObject 
{
private:
	explicit ControlsButton() = default;
	virtual ~ControlsButton() = default;

public:
	enum ControlsButtonFlag : _ullong
	{
		FlagActivate = 1 << 0,
	};
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
	static ControlsButton* Create();

private:
	Engine::BitFlag* _pBitFlag = nullptr;
	bool _isSceneChange;
	int _index = 0;
};