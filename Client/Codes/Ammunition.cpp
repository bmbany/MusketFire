#include "Ammunition.h"
#include "Mouse.h"
#include "BitFlag.h"
#include "Flintlock.h"
#include "Client_Define.h"


int Ammunition::Update(const float& fDeltaTime)
{
	SettingGravity(fDeltaTime);

	InteractionMouse();

	return 0;
}

int Ammunition::LateUpdate(const float& fDeltaTime)
{
	return 0;
}

void Ammunition::Render(Gdiplus::Graphics* pGraphics)
{
	Vector3 cameraPosition = Engine::CameraPosition();
	RenderRimLight(pGraphics);

	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);

}

void Ammunition::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void Ammunition::OnCollision(CollisionInfo info)
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

	if (*info.other == "Muzzle")
	{
		if (_pFlintlock->GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Up &&
			Engine::IsKeyPress(Input::DIM_RB) && _pFlintlock->GetCurrentState() ==
			static_cast<int>(Flintlock::ReloadAndFireStep::Spit))
		{
			_pFlintlock->OnUseAmmunition();
			Engine::MyPlaySound("SFX_SFX_PutBullet", (int)SoundGroup::SFX);

			this->SetDead();
		}
	}

	if (*info.other == "Mouse")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}
}

void Ammunition::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pItemFlag->OffFlag(FlagActivate);
	}
}

bool Ammunition::Initialize(const Vector3& position)
{
	SetName("Ammunition");
	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
	_oldScale = _pTransform->GetScale();

	_settingBattleFieldScale = Vector3(0.8f, 0.8f, 0.f);

	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));

	_vecTextures.push_back(Engine::FindTexture(L"Item_Ammunition"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pItemFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pCollider = AddCollider("Ammunition");
	_thisColliderID = _pCollider->GetID();
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(25.f, 25.f, 0.f));

	_pFlintlock = dynamic_cast<Flintlock*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Flintlock"));

	return true;
}

void Ammunition::Free()
{
}

Ammunition* Ammunition::Create(const Vector3& position)
{
	Ammunition* pInstance = new Ammunition;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
