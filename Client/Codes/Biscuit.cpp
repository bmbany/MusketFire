#include "Biscuit.h"
#include "Item.h"
#include "CockPin.h"
#include "FlashHole.h"
#include "Flintlock.h"
#include "MiniGame.h"
#include "BitFlag.h"
#include "PlayerStatus.h"
#include "Client_Define.h"
#include "Effect.h"
#include "Timer.h"

int Biscuit::Update(const float& deltaTime)
{
	InteractionMouse();

	if (!_pItemFlag->CheckFlag(FlagInInventory | FlagBattleField) && Engine::IsKeyDown(DIK_SPACE))
	{
		_pBitFlag->OnFlag(FlagEat);
		(*_pTimer)[(int)BiscuitTimer::Bite].isActive = true;
	}
	else if (Engine::IsKeyUp(DIK_SPACE))
	{
		(*_pTimer)[(int)BiscuitTimer::Bite].isActive = false;
	}

	if (!(*_pTimer)[(int)BiscuitTimer::Bite].isActive)
	{
		SettingGravity(deltaTime);
	}

	if (Engine::IsKeyPress(DIK_SPACE) && (*_pTimer)[(int)BiscuitTimer::Bite].elapsed > 1.f)
	{
		if (_pBitFlag->CheckFlag(FlagEat))
		{
			Engine::MyPlaySound("SFX_SFX_Biscuit", (int)SoundGroup::SFX);
			_pBitFlag->OffFlag(FlagEat);
		}
		_pPlayer->_isUseBiscuit = true;
	}

	if ((*_pTimer)[0].elapsed > 1.01f)
	{
		SetDead();
	}

	return 0;
}

int Biscuit::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Biscuit::Render(Gdiplus::Graphics* pGraphics)
{
	Vector3 cameraPosition = Engine::CameraPosition();

	RenderRimLight(pGraphics);

	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
}

void Biscuit::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Object, this);
}

void Biscuit::OnCollision(CollisionInfo info)
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

	if (*info.other == "Mouse")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}
}

void Biscuit::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pItemFlag->OffFlag(FlagActivate);
	}
}

bool Biscuit::CheckEat()
{
	return _pBitFlag->CheckFlag(FlagEat);
}

bool Biscuit::Initialize(const Vector3& position, PlayerStatus* player)
{
	_pPlayer = player;

	SetName("Biscuit");

	_pTransform->SetPosition(position);

	_pTransform->SetScale(Vector3(0.8f, 0.8f, 0.f));

	_oldScale = _pTransform->GetScale();

	_settingBattleFieldScale = Vector3(0.5f, 0.5f, 0.f);

	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));

	_pSpriteRenderer->SetColorKey(
		Gdiplus::Color(255, 0, 204),
		Gdiplus::Color(255, 0, 255)
	);

	_pItemFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pTimer = AddComponent<Engine::Timer>("Timer");

	_pTimer->AddTimer((int)BiscuitTimer::End);

	_vecTextures.push_back(Engine::FindTexture(L"Item_Biscuit"));

	_pCollider = AddCollider("Biscuit");
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(80.f, 50.f, 0.f));
	_thisColliderID = _pCollider->GetID();

	return true;
}

void Biscuit::Free()
{
}

Biscuit* Biscuit::Create(const Vector3& position, PlayerStatus* player)
{
	Biscuit* pInstance = new Biscuit;
	if (pInstance->Initialize(position, player))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
