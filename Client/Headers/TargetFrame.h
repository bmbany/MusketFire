#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Client_Define.h"
#include "BitFlag.h"
namespace Engine
{
	class BitFlag;
	class Collider;
	class Mouse;
}
class TargetFrame : public Engine::GameObject
{
public:
	enum Flag : _ullong
	{
		FlagLockOn = 1 << 1,
		FlagLockOff = 1 << 2
	};
protected:
	explicit TargetFrame() = default;
	virtual ~TargetFrame() = default;

public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;
protected:
	Engine::Collider*			_pCollider = nullptr;
	Engine::BitFlag*			_pBitFlag = nullptr;
	CollisionInfo _collisionInfo;
	
	Vector3						_position{};
	Vector3						_mousePosition{};
public:
	bool Initialize(const Vector3& position);
	static TargetFrame* Create(const Vector3& position);
	void Free();
	void SetPosition(const Vector3& position);

	//bool CheckFlag();
	void SetFlag(const _ullong& flag);
};
