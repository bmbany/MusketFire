#pragma once
#include "GameObject.h"

class Letterhead;
class Stamp;
class ContinueButton : public Engine::GameObject
{
private:
	explicit ContinueButton() = default;
	virtual ~ContinueButton() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;

private:
	bool Initialize();

public:
	static ContinueButton* Create();

public:
	bool _isRestart = false;

private:
	Letterhead* _pLetterhead = nullptr;
	Stamp* _pStamp = nullptr;
};

