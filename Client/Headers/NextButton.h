#pragma once
#include "GameObject.h"

class NextButton : public Engine::GameObject
{
private:
	explicit NextButton() = default;
	virtual ~NextButton() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

	void SetBattleState(bool isEnd) { _isBattleEnd = isEnd; }
	bool GetBattleState() { return _isBattleEnd; }

private:
	bool Initialize();
	
public:
	static NextButton* Create();

private:
	bool _isBattleEnd = false;
};

