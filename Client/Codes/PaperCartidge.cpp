#include "PaperCartidge.h"
#include "BitFlag.h"
#include "Flintlock.h"
#include "PaperBottom.h"
#include "Client_Define.h"

int PaperCartidge::Update(const float& deltaTime)
{
	SettingGravity(deltaTime);
	InteractionMouse();

	return 0;
}

int PaperCartidge::LateUpdate(const float& deltaTime)
{
	if (_pItemFlag->CheckFlag(FlagActivate) && !_pItemFlag->CheckFlag(FlagInInventory))
	{
		if (Engine::IsKeyDown(DIK_SPACE))
		{
			_pBitFlag->OnFlag(FlagOnSplit);
			Engine::MyPlaySound("SFX_SFX_TearPaper", (int)SoundGroup::SFX);
		}
	}

	return 0;
}

void PaperCartidge::Render(Gdiplus::Graphics* pGraphics)
{
	RenderRimLight(pGraphics);

	if (_pBitFlag->CheckFlag(FlagOnSplit))
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[2]);
		Vector3 cameraPosition = Engine::CameraPosition();
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[2]);
	}
	else
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
		Vector3 cameraPosition = Engine::CameraPosition();
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
	}
}

void PaperCartidge::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void PaperCartidge::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Inventory" && *info.itSelf == "PaperCartidgeBody")
	{
		_pItemFlag->OnFlag(FlagInInventory);
	}

	if (*info.other == "Muzzle" && _pBitFlag->CheckFlag(FlagUseable)
		&& *info.itSelf == "PaperCartidgeMiddle")
	{
		if (Engine::IsKeyDown(Input::DIM_RB) &&
			_pFlintlock->GetCurrentPivot() ==
			(int)Flintlock::FlintlockPivot::Up &&
			_pFlintlock->GetCurrentState() ==
			(int)Flintlock::ReloadAndFireStep::Puor)
		{
			_pFlintlock->OnMuzzlePowder();
			_pFlintlock->OnUseAmmunition();
			Engine::MyPlaySound("SFX_SFX_PutBullet", (int)SoundGroup::SFX);
			SetDead();
		}
	}

	if (*info.other == "Mouse" && *info.itSelf == "PaperCartidgeBody")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}
}

void PaperCartidge::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Inventory" && *info.itSelf == "PaperCartidgeBody")
	{
		_pItemFlag->OffFlag(FlagInInventory);
	}

	if (*info.other == "Mouse" && *info.itSelf == "PaperCartidgeBody")
	{
		_pItemFlag->OffFlag(FlagActivate);
	}
}

void PaperCartidge::SetUseable()
{
	_pBitFlag->OnFlag(FlagUseable);
}

bool PaperCartidge::IsOnSplit() const
{
	return _pBitFlag->CheckFlag(FlagOnSplit);
}

bool PaperCartidge::IsUseable() const
{
	return _pBitFlag->CheckFlag(FlagUseable);
}

bool PaperCartidge::Initialize(const Vector3& position)
{
	SetName("PaperCartidge");

	_pItemFlag = AddComponent<Engine::BitFlag>("ItemFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_vecTextures.push_back(Engine::FindTexture(L"Item_PaperCartidge"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pTransform->SetPosition(position);
	_pTransform->SetScale(Vector3(0.7f, 0.7f, 0.f));
	_oldScale = _pTransform->GetScale();

	_pCollider = AddCollider("PaperCartidgeBody");
	_thisColliderID = _pCollider->GetID();
	_pCollider->InitalizeOffset(Vector3(10.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(70.f, 50.f, 0.f));

	_pCollider = AddCollider("PaperCartidgeMiddle");
	_pCollider->InitalizeOffset(Vector3(-40.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(30.f, 80.f, 0.f));

	_pFlintlock = dynamic_cast<Flintlock*>(
		Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Flintlock"));

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket",
		PaperBottom::Create(this));

	return true;
}

void PaperCartidge::Free()
{
}

PaperCartidge* PaperCartidge::Create(const Vector3& position)
{
	PaperCartidge* pInstance = new PaperCartidge();
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
