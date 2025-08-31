#include "FlashHole.h"
#include "BitFlag.h"
#include "Flintlock.h"
#include "GunPowder.h"
#include "PaperCartidge.h"
#include "Client_Define.h"
#include "Mouse.h"

int FlashHole::Update(const float& fDeltaTime)
{
	UpdatePosition();
	UpdateUseGunPowder();
	UpdateUseGunPowderPaperCartidge();
	UpdateState();
	return 0;
}

int FlashHole::LateUpdate(const float& fDeltaTime)
{
	return 0;
}

void FlashHole::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetRotate(_currentDegree);
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_currentState]);

	Vector3 cameraPosition = Engine::CameraPosition();

	_pSpriteRenderer->SetTrasnform(pGraphics);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_currentState]);
	_pSpriteRenderer->ResetTransform(pGraphics);

	for (auto& Collider : GetColliders())
	{
		Collider->UpdateOffset(_currentDegree);
	}
}

void FlashHole::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Object, this);
}

void FlashHole::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "PaperCartidgeMiddle")
	{
		if (nullptr == _pPaperCartidge)
		{
			_pPaperCartidge = dynamic_cast<PaperCartidge*>(info.other->GetOwner());
		}
		else if (_pPaperCartidge->IsUseable())
		{
			_pBitFlag->OnFlag(FlagOpen);
			_pBitFlag->OnFlag(FlagPaperCartidge);
			//_pGunPowder = nullptr;
		}
	}

	if (*info.other == "GunPowderSupplies")
	{
		if (nullptr == _pGunPowder)
		{
			_pGunPowder = dynamic_cast<GunPowder*>(info.other->GetOwner());
		}
		_pBitFlag->OnFlag(FlagOpen);
		_pBitFlag->OnFlag(FlagGunPowder);
		//_pPaperCartidge = nullptr;
	}
}

void FlashHole::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "GunPowderSupplies" || *info.other == "PaperCartidgeMiddle")
	{
		_pBitFlag->OffFlag(FlagOpen);
		_pPaperCartidge = nullptr;
		_pGunPowder = nullptr;
	}
}

void FlashHole::UpdatePosition()
{
	float PositionX = _pFlintlock->GetTransform()->GetPosition().x;
	float PositionY = _pFlintlock->GetTransform()->GetPosition().y;

	float OffsetX = _pPosition->GetOffset().x;
	float OffsetY = _pPosition->GetOffset().y;

	_pTransform->SetPosition(Vector3(PositionX + OffsetX, PositionY + OffsetY, 0.f));

	_currentDegree = dynamic_cast<Flintlock*>(_pFlintlock)->GetCurrentDegree();
}

void FlashHole::UpdateState()
{
	if (_pBitFlag->CheckFlag(FlagOpen))
	{
		_currentState = (int)FlashHoleState::Closed;
	}
	else
	{
		_currentState = (int)FlashHoleState::Open;
	}
}

void FlashHole::SetStateFlag(Flag flag)
{
	_pBitFlag->OnFlag(flag);
}

void FlashHole::UpdateUseGunPowder()
{
	if (nullptr == _pGunPowder || _pGunPowder->IsDead())
	{
		return;
		//std::cout << "GunPowder is nullptr" << std::endl;
	}

	if (_pBitFlag->CheckFlag(FlagOpen))
	{
		if (_pBitFlag->CheckFlag(FlagGunPowder) && _pGunPowder->IsSuccess())
		{
			_pFlintlock->OnFlashHolePowder();
			_pGunPowder = nullptr;
		}
		else if (_pBitFlag->CheckFlag(FlagGunPowder) && _pGunPowder->IsOverCharge())
		{
			_pFlintlock->OnFlashHolePowder();
			_pFlintlock->OnOverChargePowder();
			_pGunPowder = nullptr;
		}
	}
}

void FlashHole::UpdateUseGunPowderPaperCartidge()
{
	if (nullptr == _pPaperCartidge)
	{
		return;
	}

	if (_pBitFlag->CheckFlag(FlagOpen))
	{
		if (_pBitFlag->CheckFlag(FlagPaperCartidge) &&
			_pPaperCartidge->IsUseable() &&
			Engine::IsKeyDown(Input::DIM_RB)
			)
		{
			Engine::MyPlaySound("SFX_PowderFin", (int)SoundGroup::SFX);
			_pFlintlock->OnFlashHolePowder();
			_pPaperCartidge = nullptr;
		}
	}
}

bool FlashHole::Initalize(Engine::GameObject* pFlintlock)
{
	SetName("FlashHole");

	_pFlintlock = static_cast<Flintlock*>(pFlintlock);
	_pSelectedMouse = static_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));

	_pPosition = _pFlintlock->GetCollider("FlashHolePosition");

	_vecTextures.push_back(Engine::FindTexture(L"Musket_FlashHole"));

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pTransform->SetScale(Vector3(1.f, 1.f, 1.f));

	float PositionX = _pFlintlock->GetTransform()->GetPosition().x;
	float PositionY = _pFlintlock->GetTransform()->GetPosition().y;

	float OffsetX = _pPosition->GetOffset().x;
	float OffsetY = _pPosition->GetOffset().y;

	_pTransform->SetPosition(Vector3(PositionX + OffsetX, PositionY + OffsetY, 0.f));

	_pCollider = AddCollider("FlashHole");
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(30.f, 30.f, 0.f));

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	return true;

}

void FlashHole::Free()
{
}

FlashHole* FlashHole::Create(Engine::GameObject* pFlintlock)
{
	FlashHole* pInstance = new FlashHole();
	if (pInstance->Initalize(pFlintlock))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
