#pragma once
#include "GameObject.h"
#include "Engine_Struct.h"
#include "Flintlock.h"

namespace Engine
{
	class BitFlag;
	class Animation;
	class Timer;
}
class TargetFrame;
class Flintlock;
class Mouse : public Engine::GameObject
{
public:
	enum Flag : _ullong
	{
		FlagLBUP = 1 << 1,
		FlagLBDN = 1 << 2,
		FlagRBUP = 1 << 3,
		FlagRBDN = 1 << 4,
		FlagRBPress = 1 << 5,
		FlagLBPress = 1 << 6,
		FlagSelect = 1 << 7,
		FlagLockOn = 1 << 8
	};

private:
	explicit Mouse() = default;
	virtual ~Mouse() = default;

public:
	int  Update(const float& deltaTime) override;
	int  LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
	void AddRenderer() override;
	void SelectObject(Engine::Collider* other);
	_uint GetSelectedObjectID() const;
	_uint GetPriority() const;
	Vector3 GetMousePosition();

public:
	void DisableSelectedObject();
	bool IsMouseState(Flag flag);
	bool IsSelectedObject();

public:
	bool		  Initialize();
	static Mouse* Create(); 

private:
	void Free() override;

private:
	_uint _pSelectedObject;
	_uint _priority = 0;
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;

	//created by G.J.
public:
	const int cursor_Black = 1;
	const int cursor_Grey = 0;
	TargetFrame* _pTargetFrame = nullptr;
	Flintlock* _pFlintlock = nullptr;
	
	void ChangeFlag(enum Flag);
};

