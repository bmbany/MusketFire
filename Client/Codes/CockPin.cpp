#include "CockPin.h"
#include "Flintlock.h"
#include "BitFlag.h"
#include "Section.h"
#include "Timer.h"
#include "Client_Define.h"
#include "Bubble.h"

#undef min
#undef max

int CockPin::Update(const float& deltaTime)
{
	UpdateFlintAttach();

	UpdateState(deltaTime);

	UpdatePivot(deltaTime);

	UpdatePosition();

	return 0;
}

int CockPin::LateUpdate(const float& deltaTime)
{
	return 0;
}

void CockPin::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetRotate(_currentDegree);
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_flintAttach]);

	Vector3 cameraPosition = Engine::CameraPosition();

	_pSpriteRenderer->SetTrasnform(pGraphics);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_flintAttach]);
	_pSpriteRenderer->ResetTransform(pGraphics);

	for (auto& Collider : GetColliders())
	{
		Collider->UpdateOffset(_currentDegree);
	}
}

void CockPin::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void CockPin::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Flint" && *info.itSelf == "CockPin")
	{
		if (Engine::IsKeyDown(Input::DIM_RB))
		{
			_currentDurablity = _flintDurablity;
			Engine::MyPlaySound("SFX_SFX_PutFlint", (int)SoundGroup::SFX);
		}

	}

	if (*info.other == "Mouse" && *info.itSelf == "CockPin")
	{
		_pBitFlag->OnFlag(FlagActivate);
	}
}

void CockPin::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse" && *info.itSelf == "CockPin")
	{
		_pBitFlag->OffFlag(FlagActivate);
	}
}

void CockPin::UpdateState(const float& deltaTime)
{
	if (_currentPivot == (int)CockPin::CokPinState::Idle &&
		Engine::IsKeyDown(Input::DIM_LB) &&
		_flintAttach == static_cast<int>(FlintAttach::Attached) &&
		CheckStateFlag(Flag::FlagActivate))
	{
		_currentPivot = (int)CockPin::CokPinState::Cocking;
	}
	else if (_currentPivot == (int)CockPin::CokPinState::Cocking &&
		_pFlintlock->CheckTrigger())
	{
		_currentPivot = (int)CockPin::CokPinState::Fire;
		_pTimer->SetActive(0, true);
	}
	else if (_currentPivot == (int)CockPin::CokPinState::Fire &&
		_pTimer->IsOverTime(0, 0.8f))
	{
		_currentPivot = (int)CockPin::CokPinState::Idle;
	}

	switch (_currentPivot)
	{
	case (int)CockPin::CokPinState::Idle:
		_pBitFlag->OnFlag(FlagIdle);
		_pBitFlag->OffFlag(FlagCocking);
		_pBitFlag->OffFlag(FlagFire);
		break;
	case (int)CockPin::CokPinState::Cocking:
		_pBitFlag->OffFlag(FlagIdle);
		_pBitFlag->OnFlag(FlagCocking);
		_pBitFlag->OffFlag(FlagFire);
		break;
	case (int)CockPin::CokPinState::Fire:
		_pBitFlag->OffFlag(FlagIdle);
		_pBitFlag->OffFlag(FlagCocking);
		_pBitFlag->OnFlag(FlagFire);
		break;
	default:
		break;
	}
}

void CockPin::UpdatePivot(const float& deltaTime)
{
	_currentDegree = _pFlintlock->GetCurrentDegree() + _targetDegree;

	if (_oldPivot != _currentPivot)
	{
		if (CheckStateFlag(FlagIdle))
		{
			_targetDegree = _calculateDegree[(int)CockPin::CokPinState::Idle];
		}
		else if (CheckStateFlag(FlagCocking))
		{
			_targetDegree = _calculateDegree[(int)CockPin::CokPinState::Cocking];
			Engine::MyPlaySound("SFX_SFX_CockPull", (int)SoundGroup::SFX);
		}
		else if (CheckStateFlag(FlagFire))
		{
			_targetDegree = _calculateDegree[(int)CockPin::CokPinState::Fire];
		}

		if (_currentDurablity == 0)
		{
			Engine::MyPlaySound("SFX_SFX_ThrowFlint", (int)SoundGroup::SFX);
		}
	}

	_oldPivot = _currentPivot;

}

void CockPin::UpdateFlintAttach()
{
	if (CheckStateFlag(FlagAttached) && _currentDurablity == _flintDurablity)
	{
		_flintAttach = static_cast<int>(FlintAttach::Attached);
		_pFlintlock->SettingMissFire(false);
	}
	else if (_currentDurablity == 0)
	{
		_flintAttach = static_cast<int>(FlintAttach::NotAttached);
	}
	else if (_currentDurablity < 0 ||
		_currentPivot != (int)CockPin::CokPinState::Cocking)
	{
		_pFlintlock->SettingMissFire(true);
	}
	else
	{
		_pFlintlock->SettingMissFire(false);
	}
}

void CockPin::UpdatePosition()
{
	float PositionX = _pFlintlock->GetTransform()->GetPosition().x;
	float PositionY = _pFlintlock->GetTransform()->GetPosition().y;

	float OffsetX = _pPosition->GetOffset().x;
	float OffsetY = _pPosition->GetOffset().y;

	float CalulateX = PositionX + OffsetX;
	float CalulateY = PositionY + OffsetY;

	_pTransform->SetPosition(Vector3(CalulateX, CalulateY, 0.f));
}

bool CockPin::CheckStateFlag(Flag flag)
{
	return _pBitFlag->CheckFlag(flag);
}

void CockPin::OnStateFlag(Flag flag)
{
	_pBitFlag->OnFlag(flag);
}

int CockPin::CheckDurablity() const
{
	return _currentDurablity;
}

bool CockPin::Initialize(Engine::GameObject* pFlintlock)
{
	_pFlintlock = dynamic_cast<Flintlock*>(pFlintlock);

	SetName("CockPin");

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pTimer = AddComponent<Engine::Timer>("Timer");

	_pTimer->AddTimer(1);
	_pTimer->SetActive(0, false);

	_pTransform->SetScale(Vector3(1.f, 1.f, 1.f));
	_pPosition = _pFlintlock->GetCollider("CockPinPosition");

	float PositionX = _pFlintlock->GetTransform()->GetPosition().x;
	float PositionY = _pFlintlock->GetTransform()->GetPosition().y;

	float OffsetX = _pPosition->GetOffset().x;
	float OffsetY = _pPosition->GetOffset().y;

	_pTransform->SetPosition(Vector3(PositionX + OffsetX, PositionY + OffsetY, 0.f));

	_vecTextures.push_back(Engine::FindTexture(L"Musket_Cockpin"));

	_pCollider = AddCollider("CockPin");
	_pCollider->InitalizeOffset(Vector3(0.f, -30.f, 0.f));
	_pCollider->SetScale(Vector3(30.f, 30.f, 0.f));

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pBubble = dynamic_cast<Bubble*>(Engine::MyGetObject((int)LayerGroup::UI, "Section", "Bubble"));

	return true;
}

void CockPin::Free()
{
}

CockPin* CockPin::Create(Engine::GameObject* pFlintlock)
{
	CockPin* pInstance = new CockPin;
	if (pInstance->Initialize(pFlintlock))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
