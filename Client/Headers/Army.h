#pragma once
#include "GameObject.h"

namespace Engine
{
	class Animation;
	class Timer;
	class BitFlag;
}

class Army : public Engine::GameObject
{
protected:
	explicit Army() = default;
	virtual ~Army() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) = 0;
	int LateUpdate(const float& deltaTime) = 0;
	void Render(Gdiplus::Graphics* pGraphics) = 0;
	void AddRenderer() = 0;
	virtual void SetStateDeath() = 0;
	bool IsStateDeath() { return _isStateDeath; }

protected:
	Engine::Animation*	_pAnimation = nullptr;
	Engine::Timer*		_pTimer = nullptr;
	Engine::BitFlag*	_pBitFlag = nullptr;
	bool				_isStateDeath = false;
};

