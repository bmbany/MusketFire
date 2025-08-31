#include "Rammer.h"
#include "Mouse.h"
#include "Timer.h"
#include "Flintlock.h"
#include "Client_Define.h"
#include "BitFlag.h"

#include <cmath>

int Rammer::Update(const float& fDeltaTime)
{
	const float ScreenHeight = 720.f;
	const float CombatZoneTop = 0.f;
	const float CombatZoneBottom = 300.f;
	const float CombatZoneGround = 255.f;

	ProcedureRamSlot();

	if (RammerStateFlag(flag::FlagRammingPosition) || RammerStateFlag(flag::FlagInSlot) ||
		_pItemFlag->CheckFlag(ItemFlag::FlagInInventory))
	{
		_currentGravity = 0.f;
	}
	else
	{
		_currentGravity = _gravity;
		if (ScreenHeight > _pTransform->GetPosition().y && CombatZoneBottom <= _pTransform->GetPosition().y)
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x,
				_pTransform->GetPosition().y + _currentGravity * fDeltaTime, 0.f));
			_pTransform->SetScale(Vector3(1.f, 1.f, 1.f));
		}
		else if (ScreenHeight <= _pTransform->GetPosition().y ||
			CombatZoneTop <= _pTransform->GetPosition().y ||
			CombatZoneBottom > _pTransform->GetPosition().y)
		{
			if (_pTransform->GetPosition().y <= CombatZoneGround)
			{
				_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x,
					CombatZoneGround, 0.f));
			}
			else
			{
				_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x * 0.5f,
					CombatZoneGround, 0.f));
			}
			if(!_pBitFlag->CheckFlag(flag::FlagRammingPosition))
				_pTransform->SetScale(Vector3(0.5f, 0.5f, 1.f));
		}
	}

	UpdatePivot(fDeltaTime);

	CheckStateFlag(fDeltaTime);

	if (_pItemFlag->CheckFlag(ItemFlag::FlagActivate) &&
		!RammerStateFlag(flag::FlagInSlot) &&
		!RammerStateFlag(flag::FlagRammingPosition))
	{
		InteractionMouse();
	}

	return 0;
}

int Rammer::LateUpdate(const float& fDeltaTime)
{

	return 0;
}

void Rammer::Render(Gdiplus::Graphics* pGraphics)
{
	if (RammerStateFlag(flag::FlagRenderAttached))
	{
		_currentState = static_cast<int>(RenderClip::Attached);
	}
	else
	{
		_currentState = static_cast<int>(RenderClip::Idle);
	}

	_pSpriteRenderer->SetRotate(_currentDegree);
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_currentState]);

	_pSpriteRenderer->SetTrasnform(pGraphics);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_currentState]);
	_pSpriteRenderer->ResetTransform(pGraphics);

	for (auto& Collider : GetColliders())
	{
		Collider->UpdateOffset(_currentDegree);
	}
}

void Rammer::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::BackObject, this);
}

void Rammer::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Mouse" && *info.itSelf == "RammerBody")
	{
		_pItemFlag->OnFlag(ItemFlag::FlagActivate);
	}

	if (*info.other == "Inventory" && *info.itSelf == "RammerBody")
	{
		_pItemFlag->OnFlag(ItemFlag::FlagInInventory);
	}
	if (*info.other == "Musket" && *info.itSelf == "RammerBody")
	{
		_pItemFlag->OffFlag(ItemFlag::FlagInInventory);
	}
	if (*info.other == "RammerSlot" && *info.itSelf == "RammerEnd" &&
		_pFlintlock->GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Up &&
		/*GetRammingDegree() == _maxDegree &&*/ Engine::IsKeyPress(Input::DIM_RB)) // 램 슬롯 수납 각도 제한1
	{
		_pBitFlag->OnFlag(flag::FlagInSlot);
		_pBitFlag->OnFlag(flag::FlagProcedureInSlot);
	}
}

void Rammer::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "RammerSlot" && *info.itSelf == "RammerEnd")
	{
		_pBitFlag->OffFlag(flag::FlagProcedureInSlot);
	}

	if (*info.other == "Mouse" && *info.itSelf == "RammerBody")
	{
		_pItemFlag->OffFlag(ItemFlag::FlagActivate);
	}
}

void Rammer::OnStateFlag(flag flag)
{
	_pBitFlag->OnFlag(flag);
}

void Rammer::OffStateFlag(flag flag)
{
	_pBitFlag->OffFlag(flag);
}

bool Rammer::RammerStateFlag(flag flag)
{
	return _pBitFlag->CheckFlag(flag);
}

void Rammer::CheckStateFlag(const float& fDeltaTime)
{
	if (RammerStateFlag(flag::FlagInSlot))
	{
		_currentDegree = _pFlintlock->GetCurrentDegree();
		RammerLerps(fDeltaTime);
	}
	else if (RammerStateFlag(flag::FlagRammingPosition))
	{
		const float InverseDegree = 165.f;
		_currentDegree = _pFlintlock->GetCurrentDegree() + InverseDegree;

		RammerLerps(fDeltaTime);
	}
	else if (!RammerStateFlag(flag::FlagInSlot))
	{
		for (auto& Collider : GetColliders())
		{
			Collider->SetActive(true);
		}
	}
}

void Rammer::RammerLerps(const float& fDeltaTime)
{
	for (auto& Collider : GetColliders())
	{
		Collider->SetActive(false);
	}

	if (RammerStateFlag(flag::FlagProcedureInSlot))
	{
		_currentDegree = _pFlintlock->GetCurrentDegree() - 15.f;

		(*_pTimer)[0].isActive = true;
		Vector3 FlintlockOffset = _pFlintlock->GetCollider("RammerSlot")->GetOffset();
		Vector3 RammerEndOffset = FindCollider("RammerEnd")->GetOffset();

		if (_pTimer->IsBetweenTime(static_cast<int>(RammerTimer::Disabled), 0.1f, 0.2f))
		{
			_pSelectedMouse->DisableSelectedObject();
			_pTransform->SetPosition(Vector3(
				_pFlintlock->GetTransform()->GetPosition().x
				+ FlintlockOffset.x - RammerEndOffset.x,
				_pFlintlock->GetTransform()->GetPosition().y
				+ FlintlockOffset.y - RammerEndOffset.y, 0.f));
		}

		float offsetX = _pFlintlock->GetTransform()->GetPosition().x;
		float offsetY = _pFlintlock->GetTransform()->GetPosition().y;

		float lerpX = _pTransform->GetPosition().x - offsetX;
		float lerpY = _pTransform->GetPosition().y - offsetY;
		float distance = sqrt((lerpX * lerpX) + (lerpY * lerpY));
		float tolerance = 0.1f;  // 근접 거리 허용 범위 설정
		float inSlotSpeed = 18.5f;
		// 거리가 허용 범위보다 작으면 래밍 멈춤
		if (distance <= tolerance)
		{
			// 래밍을 멈추는 로직 추가
			_pTransform->SetPosition(Vector3(offsetX, offsetY, 0.f));
			Engine::MyPlaySound("SFX_SFX_InStick", (int)SoundGroup::SFX);
			_pBitFlag->OffFlag(FlagProcedureInSlot);

		}
		else if (_pTimer->IsOverTime(static_cast<int>(RammerTimer::Disabled), 0.4f))
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x - lerpX * inSlotSpeed * fDeltaTime,
				_pTransform->GetPosition().y - lerpY * inSlotSpeed * fDeltaTime, 0.f));
		}

		if (_pTimer->IsOverTime(static_cast<int>(RammerTimer::Disabled), 0.4f))
		{
			_currentDegree = _pFlintlock->GetCurrentDegree();
			_pBitFlag->OnFlag(flag::FlagInSlot);
			_pBitFlag->OnFlag(flag::FlagRenderAttached);
		}
	}
	else if (RammerStateFlag(flag::FlagRammingPosition))
	{
		(*_pTimer)[0].isActive = true;
		Vector3 FlintlockOffset = _pFlintlock->GetCollider("Muzzle")->GetOffset();
		Vector3 RammerEndOffset = FindCollider("RammerHead")->GetOffset();

		if (_pTimer->IsBetweenTime(static_cast<int>(RammerTimer::Disabled), 0.1f, 0.2f))
		{
			_pSelectedMouse->DisableSelectedObject();

			_pTransform->SetPosition(Vector3(
				_pFlintlock->GetTransform()->GetPosition().x
				+ FlintlockOffset.x - RammerEndOffset.x,
				_pFlintlock->GetTransform()->GetPosition().y
				+ FlintlockOffset.y - RammerEndOffset.y, 0.f));

			_rammingStartPosition = _pTransform->GetPosition();
		}

		int offsetX = int(_pFlintlock->GetTransform()->GetPosition().x +
			_pFlintlock->GetCollider("Muzzle")->GetOffset().x);
		int offsetY = int(_pFlintlock->GetTransform()->GetPosition().y +
			_pFlintlock->GetCollider("Muzzle")->GetOffset().y);

		float lerpX = _pTransform->GetPosition().x - offsetX;
		float lerpY = _pTransform->GetPosition().y - offsetY;

		float reverseLerpX = _pTransform->GetPosition().x - (int)_rammingStartPosition.x;
		float reverseLerpY = _pTransform->GetPosition().y - (int)_rammingStartPosition.y;

		// 현재 위치와 목표 위치 사이의 거리 계산
		float distance = sqrt((lerpX * lerpX) + (lerpY * lerpY));
		float reverseDistance = sqrt((reverseLerpX * reverseLerpX) + (reverseLerpY * reverseLerpY));
		float tolerance = 5.f;  // 근접 거리 허용 범위 설정
		float rammingSpeed = 6.5f;

		// 거리가 허용 범위보다 작으면 래밍 멈춤
		if (distance <= tolerance && !RammerStateFlag(flag::FlagCompleteRamming))
		{
			// 래밍을 멈추는 로직 추가
			_pTransform->SetPosition(Vector3(float(offsetX), float(offsetY), 0.f));
			_pBitFlag->OnFlag(flag::FlagCount);
			_pBitFlag->OnFlag(flag::FlagCompleteRamming);

			Engine::MyPlaySound("SFX_SFX_PushStick", (int)SoundGroup::SFX);
		}
		else if (reverseDistance <= tolerance && RammerStateFlag(flag::FlagCompleteRamming))
		{
			_pTransform->SetPosition(Vector3(_rammingStartPosition.x, _rammingStartPosition.y, 0.f));
			_pBitFlag->OffFlag(flag::FlagCompleteRamming);

			Engine::MyPlaySound("SFX_SFX_StickSwing", (int)SoundGroup::SFX);
		}
		else if (!RammerStateFlag(flag::FlagCompleteRamming) && Engine::IsKeyPress(Input::DIM_RB))
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x - lerpX * rammingSpeed * fDeltaTime,
				_pTransform->GetPosition().y - lerpY * rammingSpeed * fDeltaTime, 0.f));
		}
		else if (RammerStateFlag(flag::FlagCompleteRamming))
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x - reverseLerpX * rammingSpeed * fDeltaTime,
				_pTransform->GetPosition().y - reverseLerpY * rammingSpeed * fDeltaTime, 0.f));
		}
	}
}

bool Rammer::Initialize(const Vector3& position, Engine::GameObject* pFlintlock)
{
	SetName("Rammer");

	_pSelectedMouse = dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));
	_pFlintlock = dynamic_cast<Flintlock*>(pFlintlock);

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(static_cast<int>(RammerTimer::End));

	_pTransform->SetScale(Vector3(1.f, 1.f, 1.f));
	_pTransform->SetPosition(_pFlintlock->GetTransform()->GetPosition());
	_currentDegree = _pFlintlock->GetCurrentDegree();

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pItemFlag = AddComponent<Engine::BitFlag>("ItemFlag");

	_pBitFlag->OnFlag(flag::FlagRenderAttached);
	_pBitFlag->OnFlag(flag::FlagInSlot);

	_vecTextures.push_back(Engine::FindTexture(L"Musket_Rammer"));

	_pCollider = AddCollider("RammerBody");
	_thisColliderID = _pCollider->GetID();
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(400.f, 100.f, 0.f));

	_pCollider = AddCollider("RammerEnd");
	_pCollider->InitalizeOffset(Vector3(-180.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(20.f, 20.f, 0.f));

	_pCollider = AddCollider("RammerHead");
	_pCollider->InitalizeOffset(Vector3(180.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(20.f, 20.f, 0.f));

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	return true;
}

void Rammer::Free()
{
}

void Rammer::UpdatePivot(const float& fDeltaTime)
{
	if (!_pSelectedMouse)
	{
		return;
	}

	if (_pSelectedMouse->IsMouseState(Mouse::Flag::FlagLBDN) &&
		!RammerStateFlag(Rammer::flag::FlagInSlot))
	{
		if (!RammerStateFlag(Rammer::flag::FlagInSlot) &&
			!RammerStateFlag(flag::FlagRammingPosition) &&
			Engine::IsMouseWheel(VK_MWHEELDOWN))
		{
			_currentDegree += _rotationSpeed * _rotationDegree * fDeltaTime;

			if (_currentDegree > _maxDegree) {
				_currentDegree = _maxDegree;
			}
		}
		if (!RammerStateFlag(Rammer::flag::FlagInSlot) &&
			!RammerStateFlag(flag::FlagRammingPosition) &&
			Engine::IsMouseWheel(VK_MWHEELUP))
		{
			_currentDegree -= _rotationSpeed * _rotationDegree * fDeltaTime;

			if (_currentDegree < _minDegree) {
				_currentDegree = _minDegree;
			}
		}
	}
}

void Rammer::ProcedureRamSlot()
{
	if (!RammerStateFlag(flag::FlagInSlot))
	{
		if (RammerStateFlag(flag::FlagProcedureInSlot))
		{
			_pFlintlock->SettingIsCollisionTrigger(true);
		}
		else
		{
			_pFlintlock->SettingIsCollisionTrigger(false);
		}
	}

	if (!RammerStateFlag(flag::FlagInSlot) && !RammerStateFlag(flag::FlagRammingPosition))
	{
		(*_pTimer)[0].isActive = false;
	}

}

Rammer* Rammer::Create(const Vector3& position, Engine::GameObject* Flintlock)
{
	Rammer* pInstance = new Rammer();
	if (pInstance->Initialize(position, Flintlock))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
