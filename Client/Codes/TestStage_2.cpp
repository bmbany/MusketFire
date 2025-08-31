#ifdef __YOUNGUNG__

#include "TestStage_2.h"
#include "Mouse.h"

// Musket
#include "Flintlock.h"
#include "Ammunition.h"
#include "CockPin.h"
#include "FlashHole.h"
#include "Flint.h"

// Item
#include "Rammer.h"
#include "GunPowder.h"
#include "PaperCartidge.h"
#include "FireStepText.h"
#include "Letter.h"
#include "Biscuit.h"

// Section
#include "Inventory.h"
#include "Musket.h"
#include "Combat.h"
#include "UI.h"
#include "Bubble.h"

#include "CombatManager.h"
#include "CollisionManager.h"
#include "Client_Define.h"

int TestStage::Update(const float& fDeltaTime)
{
	_elapsed += fDeltaTime;
	_fps++;

	if (1.f <= _elapsed)
	{
		std::cout << _fps << std::endl;
		_elapsed = 0.f;
		_fps = 0;
	}

	return 0;
}

int TestStage::LateUpdate(const float& fDeltaTime)
{
	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
		Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"),
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "Section"),
		Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "Section"),
		Engine::GetObjectList((int)LayerGroup::Object, "Musket"));

	_pCollisionMgr->CheckCollision(
		Engine::GetObjectList((int)LayerGroup::UI, "UI"),
		Engine::GetObjectList((int)LayerGroup::Object, "Mouse"));

	return 0;
}

void TestStage::Free()
{
	Engine::SafeRelease(_pCollisionMgr);
	Engine::SafeRelease(_pCombatMgr);
}

bool TestStage::Initialize()
{
	_pCollisionMgr = Engine::CollisionManager::Create();
	_pCombatMgr = CombatManager::GetInstance();

	//Engine::AddObjectInLayer((int)LayerGroup::Object, "Resource", CheckResource::Create());

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Mouse", Mouse::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Bubble::Create(Vector3(500.f, 300.f, 0.f), 1));

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Flintlock::Create(Vector3(600.f, 540.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 350.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 350.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 350.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", PaperCartidge::Create(Vector3(150.f, 550.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", GunPowder::Create(Vector3(150.f, 600.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Letter::Create(Vector3(150.f, 550.f, 0.f), 2));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Biscuit::Create(Vector3(150.f, 580.f, 0.f)));

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Text", FireStepText::Create(Vector3(930.f, 500.f, 0.f)));

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Combat::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Musket::Create());
	Engine::AddObjectInLayer((int)LayerGroup::UI, "Section", Inventory::Create());


	UI::GDI_Info info;
	info.textureTag = L"UI_Frame";
	info.index = 1;
	info.position = { 640.f, 508.f, 0.f };
	info.scale = { 1.f, 1.f, 0.f };
	info.name = "Frame";
	info.originHDC = Engine::GetOriginHDC();
	info.backHDC = Engine::GetBackHDC();
	info.isTransparent = true;
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", UI::Create(info));

	return true;
}

TestStage* TestStage::Create()
{
	TestStage* pInstance = new TestStage;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
#endif