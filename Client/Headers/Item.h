#pragma once
#include "GameObject.h"
#include <Engine_Struct.h>
#include "Client_Define.h"
#include "Mouse.h"

namespace Engine
{
	class BitFlag;
	class Collider;
	class Animation;
	class Timer;
}

class Item : public Engine::GameObject
{
public:
	enum ItemFlag : _ullong
	{
		FlagActivate = 1 << 0,
		FlagInInventory = 1 << 1,
		FlagBattleField = 1 << 2,
	};

protected:
	explicit Item() = default;
	virtual ~Item() = default;

public:
	virtual int Update(const float& deltaTime) override = 0;
	virtual int LateUpdate(const float& deltaTime) override = 0;
	virtual void Render(Gdiplus::Graphics* pGraphics) override = 0;
	virtual void AddRenderer() override = 0;
	virtual void OnCollision(CollisionInfo info) override = 0;
	virtual void OnCollisionExit(CollisionInfo info) override = 0;

public:
	_uint GetPriority() { return _priority; }
	void SettingGravity(const float& deltaTime);
	void InteractionMouse();
	void RenderRimLight(Gdiplus::Graphics* pGraphics);

protected:
	Engine::BitFlag* _pItemFlag = nullptr;
	Engine::Collider* _pCollider = nullptr;
	Mouse* _pSelectedMouse =
		dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));

protected:
	const float _gravity = 9.8f;

protected:
	_uint _thisColliderID = 0;
	_uint _priority = 0;
	float _currentGravity = 0.f;
	Vector3 _oldScale{};
	Vector3 _settingBattleFieldScale{ 0.5f, 0.5f, 0.5f };
	Vector3 _offset{};
	Vector3 _mousePosition{};
	Vector3 _newPosition{};
};

