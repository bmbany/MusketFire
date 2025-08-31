#include "BattleField.h"
#include "MapEffect.h"

// Section
#include "Combat.h"
#include "Inventory.h"
#include "Musket.h"
#include "Frame.h"
#include "Bubble.h"
#include "Letterhead.h"

// UI
#include "Mouse.h"

#include "CollisionManager.h"
#include "Client_Define.h"

int BattleField::Update(const float& deltaTime)
{
	return 0;
}

int BattleField::LateUpdate(const float& deltaTime)
{
	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
		Engine::GetObjectList((int)LayerGroup::UI, "Mouse"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "Section"),
		Engine::GetObjectList((int)LayerGroup::UI, "Mouse"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "Section"),
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "UI"),
		Engine::GetObjectList((int)LayerGroup::UI, "Mouse"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "UI"),
		Engine::GetObjectList((int)LayerGroup::UI, "UI"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::Object, "Soldier"),
		Engine::GetObjectList((int)LayerGroup::Object, "Soldier"));

	return 0;
}

bool BattleField::Initialize()
{
	_pCollisionMgr = Engine::CollisionManager::Create();

    Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", MapEffect::Create(Vector3()));
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Mouse", Mouse::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Bubble::Create(Vector3(500.f, 300.f, 0.f), 1));

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Inventory::Create());	
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Combat::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Musket::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Frame::Create());

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section",
		Letterhead::Create(Vector3(620.f, 330.f, -110.f), 1, nullptr));

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section",
		Letterhead::Create(Vector3(660.f, 390.f, -100.f), 6, nullptr));

	return true;
}

void BattleField::Free()
{
	Engine::SafeRelease(_pCollisionMgr);
}

BattleField* BattleField::Create()
{
	return new BattleField;
}
