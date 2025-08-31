#pragma once
#include "GameObject.h"

namespace Engine
{
	class BitFlag;
	class Animation;
	class Collider;
}

class MusketObject : public Engine::GameObject
{
public:
	enum ItemFlag : _ullong
	{
		FlagActivate = 1 << 0,
		FlagInInventory = 1 << 1,
	};

protected:
	explicit MusketObject() = default;
	virtual ~MusketObject() = default;

public:
	virtual int  Update(const float& fDeltaTime) override = 0;
	virtual int  LateUpdate(const float& fDeltaTime) override = 0;
	virtual void Render(Gdiplus::Graphics* pGraphics) override = 0;
	virtual void AddRenderer() override = 0;

protected:
	Engine::BitFlag* _pItemFlag = nullptr;

protected:
	const float _gravity = 9.8f;

protected:
	float _currentGravity = 0.f;
};
