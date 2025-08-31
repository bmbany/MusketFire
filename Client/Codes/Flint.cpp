#include "Flint.h"
#include "Item.h"
#include "Flintlock.h"
#include "CockPin.h"
#include "Mouse.h"
#include "BitFlag.h"
#include "Client_Define.h"

int Flint::Update(const float& deltaTime)
{

	SettingGravity(deltaTime);

	InteractionMouse();
	return 0;
}

int Flint::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Flint::Render(Gdiplus::Graphics* pGraphics)
{
	RenderRimLight(pGraphics);

	Vector3 cameraPosition = Engine::CameraPosition();
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);

}

void Flint::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void Flint::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Inventory")
	{
		_pItemFlag->OnFlag(FlagInInventory);
	}
	if (*info.other == "Musket")
	{
		_pItemFlag->OffFlag(FlagInInventory);
	}
	if (*info.other == "CockPin")
	{
		if (_pFlintlock->GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Middle &&
			_pSelectedMouse->IsMouseState(Mouse::Flag::FlagRBPress))
		{
			_pCockPin->OnStateFlag(CockPin::Flag::FlagAttached);
			Engine::MyPlaySound("SFX_SFX_PutFlint", (int)SoundGroup::SFX);

			this->SetDead();
		}
	}
	if (*info.other == "Mouse")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}

}

void Flint::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pItemFlag->OffFlag(Item::ItemFlag::FlagActivate);
	}
}

bool Flint::Initialize(const Vector3& position)
{
	SetName("Flint");
	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));

	_pItemFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_vecTextures.push_back(Engine::FindTexture(L"Item_Flint"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pCollider = AddCollider("Flint");
	_thisColliderID = _pCollider->GetID();
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(25.f, 25.f, 0.f));

	_pCockPin = dynamic_cast<CockPin*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "CockPin"));
	_pFlintlock = dynamic_cast<Flintlock*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Flintlock"));
	
	_settingBattleFieldScale = { 1.f, 1.f, 0.f };
	return true;
}

void Flint::Free()
{
}

Flint* Flint::Create(const Vector3& position)
{
	Flint* pInstance = new Flint;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
