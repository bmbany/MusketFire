#include "Flintlock.h"

#include "Mouse.h"
#include "Rammer.h"
#include "CockPin.h"
#include "FlashHole.h"
#include "GunPowder.h"

#include "Bubble.h"

#include "BitFlag.h"
#include "Timer.h"
#include "Client_Define.h"

#include <random>
#include <ctime>

#undef min
#undef max

int Flintlock::Update(const float& fDeltaTime)
{
	UpdatePivot(fDeltaTime);

	UpdateTrigger();

	if (!_pBubble->IsBubbleRendering(Bubble::Alies))
	{
		(*_pTimer)[DelayLoading].isActive = true;
	}
	else
	{
		(*_pTimer)[DelayLoading].isActive = false;
	}

	if (_currentState == (int)ReloadAndFireStep::Aim)
	{
		(*_pTimer)[DelayFire].isActive = true;
	}
	else
	{
		(*_pTimer)[DelayFire].isActive = false;
	}

	switch (_currentState)
	{
	case (int)ReloadAndFireStep::Puor:
		UpdatePuor();
		break;
	case (int)ReloadAndFireStep::Spit:
		UpdateSpit();
		break;
	case (int)ReloadAndFireStep::Ramming:
		UpdateRamming();
		break;
	case (int)ReloadAndFireStep::Aim:
		UpdateAim();
		break;
	default:
		break;
	}

	_oldState = _currentState;

	return 0;
}

int Flintlock::LateUpdate(const float& fDeltaTime)
{
	if (_currentPivot == (int)FlintlockPivot::Down)
	{
		_currentState = (int)ReloadAndFireStep::Puor;
		_pBitFlag->ResetFlag();
		_rammerStack = (int)RammerStack::Idle; //버그 수정1
	}

	if (_currentPivot == (int)FlintlockPivot::Down &&
		_currentState == (int)ReloadAndFireStep::Puor)
	{
		_pBubble->BubbleRangedSpeak(Bubble::Alies, 25, 27);
	}

	UpdateFireCondition();

	if (_currentState != (int)ReloadAndFireStep::Aim &&
		12.f < (*_pTimer)[DelayLoading].elapsed)
	{
		_pBubble->BubbleRangedSpeak(Bubble::Alies, 35, 38);
	}
	else if (_currentState == (int)ReloadAndFireStep::Aim &&
		12.f < (*_pTimer)[DelayLoading].elapsed)
	{
		_pBubble->BubbleRangedSpeak(Bubble::Alies, 39, 41);
	}

	return 0;
}

void Flintlock::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetRotate(_currentDegree);
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);

	Vector3 cameraPosition = Engine::CameraPosition();

	_pSpriteRenderer->SetTrasnform(pGraphics);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
	_pSpriteRenderer->ResetTransform(pGraphics);

	for (auto& Collider : GetColliders())
	{
		Collider->UpdateOffset(_currentDegree);
	}
}

void Flintlock::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::BackObject, this);
}

void Flintlock::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
	{
		_pBitFlag->ResetFlag();
		_pBitFlag->OnFlag(FlagRestarting);
		return;
	}
	else
	{
		_pBitFlag->OffFlag(FlagRestarting);
	}

	if (*info.other == "Rammer")
	{
		if (nullptr == _pRammer)
		{
			_pSelectedMouse = dynamic_cast<Mouse*>(info.other->GetOwner());
		}
	}

	_collisionInfo.itSelf = info.itSelf;

	ProcedureRamSlot(info.other);

	switch (_currentState)
	{
	case (int)ReloadAndFireStep::Puor:
		ProcedurePuor(info.other);
		break;
	case (int)ReloadAndFireStep::Ramming:
		ProcedureRamming(info.other);
		break;
	default:
		break;
	}
}

void Flintlock::OnCollisionExit(CollisionInfo info)
{
	_collisionInfo.itSelf = nullptr;

	if ((*info.other) == "Rammer")
	{
		_pRammer = nullptr;
	}
	else if ((*info.other) == "GunPowderSupplies")
	{
		_pGunPowder = nullptr;
	}
	else if ((*info.other) == "Mouse")
	{
		_pSelectedMouse = nullptr;
	}
}

void Flintlock::ProcedureRamSlot(Engine::Collider* other)
{
	if ((*other->GetOwner()) == "Rammer" && isCollisionTrigger)
	{
		if (nullptr == _pRammer)
		{
			_pRammer = dynamic_cast<Rammer*>(other->GetOwner());
		}

		if (!_pRammer->RammerStateFlag(Rammer::flag::FlagInSlot))
		{
			if ((*other) == "RammerEnd")
			{
				_pRammer->OnStateFlag(Rammer::flag::FlagInSlot);
			}
		}
	}
	else if ((*other) == "Mouse" && (*_collisionInfo.itSelf) == "RammerSlot" &&
		GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Up &&
		Engine::IsKeyPress(Input::DIM_LB))
	{
		if (_pRammer->IsDead() || nullptr == _pRammer)
		{
			return;
		}

		if (_pRammer->RammerStateFlag(Rammer::flag::FlagInSlot))
		{
			_pRammer->OffStateFlag(Rammer::flag::FlagInSlot);
			_pRammer->OffStateFlag(Rammer::flag::FlagRammingPosition);
			_pRammer->OffStateFlag(Rammer::flag::FlagRenderAttached);

			float TargetX = other->GetOwner()->GetTransform()->GetPosition().x;
			float TargetY = other->GetOwner()->GetTransform()->GetPosition().y;

			_pRammer->GetTransform()->SetPosition(Vector3(TargetX, TargetY, 0.f));
			Engine::MyPlaySound("SFX_SFX_OutStick", (int)SoundGroup::SFX);
		}
	}
	else
	{
		isCollisionTrigger = false;
	}
}

void Flintlock::ProcedurePuor(Engine::Collider* other)
{
	if ((*other) == "GunPowderSupplies" && (*_collisionInfo.itSelf) == "Muzzle")
	{
		if (_pGunPowder == nullptr)
		{
			_pGunPowder = dynamic_cast<GunPowder*>(other->GetOwner());
		}
		//minigame!
		if (GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Up &&
			Engine::IsKeyUp(Input::DIM_RB))
		{
			if (_pGunPowder->IsSuccess())
			{
				_pBitFlag->OnFlag(FlagMuzzlePowder);
			}
			else if (_pGunPowder->IsOverCharge())
			{
				_pBitFlag->OnFlag(FlagOverChargePowder | FlagMuzzlePowder);
			}
			else if (_pGunPowder->IsFail())
			{
				_pBitFlag->OffFlag(FlagMuzzlePowder | FlagOverChargePowder);
			}
		}
	}
}

void Flintlock::ProcedureRamming(Engine::Collider* other)
{
	if ((*other->GetOwner()) == "Rammer")
	{
		if (!_pRammer->RammerStateFlag(Rammer::flag::FlagInSlot) &&
			_rammerStack < (int)RammerStack::Max)
		{
			if ((*other) == "RammerHead" && (*_collisionInfo.itSelf) == "Muzzle"/* &&
				_pRammer->GetRammingDegree() == 165.f*/)
			{
				_pRammer->OnStateFlag(Rammer::flag::FlagRammingPosition);
				_pBubble->BubbleSpeak(Bubble::Alies, 29);
			}
		}
	}
}

void Flintlock::UpdatePuor()
{
	SettingMissFire(true);

	if (_pBitFlag->CheckFlag(FlagMuzzlePowder) && _pBitFlag->CheckFlag(FlagFlashHolePowder))
	{
		_currentState = (int)ReloadAndFireStep::Spit;
	}
}

void Flintlock::UpdateSpit()
{
	if (_pBitFlag->CheckFlag(FlagUseAmmunition))
	{
		_currentState = (int)ReloadAndFireStep::Ramming;
	}
}

void Flintlock::UpdateRamming()
{
	if (_pRammer->IsDead() || nullptr == _pRammer)
	{
		return;
	}
	else if (_rammerStack == (int)RammerStack::Idle)
	{
		_rammerStack = RandomRammerStack();
	}
	if (_pRammer->RammerStateFlag(Rammer::flag::FlagCount))
	{
		_rammerStack++;

		_pRammer->OffStateFlag(Rammer::flag::FlagCount);
	}
	else if (_rammerStack == (int)RammerStack::Max &&
		!_pRammer->RammerStateFlag(Rammer::flag::FlagCompleteRamming))
	{
		_pRammer->OffStateFlag(Rammer::flag::FlagRammingPosition);
		_pRammer->OffStateFlag(Rammer::flag::FlagCompleteRamming);
		_pRammer->OffStateFlag(Rammer::flag::FlagCount);
		//testcode
		_pRammer->OffStateFlag(Rammer::flag::FlagInSlot);

		_pBitFlag->OnFlag(FlagRamComplete);

		Engine::MyPlaySound("SFX_SFX_PushStickFin", (int)SoundGroup::SFX);

		_currentState = (int)ReloadAndFireStep::Aim;
	}
}

void Flintlock::UpdateAim()
{
	if (_currentPivot == (int)FlintlockPivot::Middle && CheckTrigger())
	{
		if (3.5f > (*_pTimer)[DelayFire].elapsed)
		{
			if (Engine::IsKeyUp(Input::DIM_RB))
			{
				_pBubble->BubbleSpeak(Bubble::Alies, 31);
			}
			else if (Engine::IsKeyPress(Input::DIM_RB))
			{
				_pBubble->BubbleRangedSpeak(Bubble::Alies, 32, 34);
			}
		}

		if (Engine::IsKeyPress(Input::DIM_RB))
		{
			if (Engine::IsKeyDown(Input::DIM_LB))
			{
				if (_pBitFlag->CheckFlag(FlagMissFire))
				{
					_pBitFlag->OffFlag(FlagAiming | FlagFire);
				}
				else
				{
					_pBitFlag->OnFlag(FlagAiming | FlagFire);
				}
			}
		}
		else if (Engine::IsKeyUp(Input::DIM_RB))
		{
			_pBitFlag->OffFlag(FlagAiming);
		}
	}
}

void Flintlock::UpdateFireCondition()
{
	if (_pBitFlag->CheckFlag(FlagFire))
	{
		_pBitFlag->ResetFlag();

		_rammerStack = (int)RammerStack::Idle;

		_currentState = (int)ReloadAndFireStep::Puor;
	}
}

void Flintlock::UpdateTrigger()
{
	if (_currentPivot == (int)Flintlock::FlintlockPivot::Middle)
	{
		if (Engine::IsKeyPress(Input::DIM_RB))
		{
			if (Engine::IsKeyPress(Input::DIM_LB))
			{
				_pBitFlag->OnFlag(FlagTrigger);

				if ((_currentState != (int)Flintlock::ReloadAndFireStep::Puor &&
					_currentState != (int)Flintlock::ReloadAndFireStep::Aim) &&
					_pCockPin->CheckStateFlag(CockPin::FlagFire) &&
					_pBitFlag->CheckFlag(FlagTrigger))
				{
					_pBubble->BubbleRangedSpeak(Bubble::Alies, 28, 30);
				}
			}
		}
		else if (Engine::IsKeyUp(Input::DIM_RB))
		{
			_pBitFlag->OffFlag(FlagTrigger);
		}
	}

}

void Flintlock::UpdatePivot(const float& fDeltaTime)
{
	if (_pBitFlag->CheckFlag(FlagRestarting))
		return;

	if (_pRammer->RammerStateFlag(Rammer::FlagProcedureInSlot) || _pRammer->RammerStateFlag(Rammer::FlagRammingPosition))
		return;

	if (Engine::IsKeyDown(DIK_Q))
	{
		int nextPivot = _currentPivot + 1;
		if (nextPivot < (int)FlintlockPivot::Max) {
			_oldPivot = _currentPivot;
			_currentPivot = nextPivot;
		}
	}
	if (Engine::IsKeyDown(DIK_E))
	{
		int prevPivot = _currentPivot - 1;
		if (prevPivot > (int)FlintlockPivot::Min) {
			_oldPivot = _currentPivot;
			_currentPivot = prevPivot;
		}
	}

	switch (_currentPivot)
	{
	case (int)Flintlock::FlintlockPivot::Down:
		_targetDegree = (float)Flintlock::PivotDegree::Down;
		break;
	case (int)Flintlock::FlintlockPivot::Middle:
		_targetDegree = (float)Flintlock::PivotDegree::Middle;
		break;
	case (int)Flintlock::FlintlockPivot::Up:
		_targetDegree = (float)Flintlock::PivotDegree::Up;
		break;
	}

	if (_currentDegree < _targetDegree)
		_currentDegree = std::min(_currentDegree + _rotationSpeed * fDeltaTime, _targetDegree);
	else if (_currentDegree > _targetDegree)
		_currentDegree = std::max(_currentDegree - _rotationSpeed * fDeltaTime, _targetDegree);
}

int Flintlock::RandomRammerStack()
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib((int)RammerStack::Two, (int)RammerStack::Four);
	//std::cout << distrib(gen) << std::endl;
	return distrib(gen);
}

void Flintlock::OnUseAmmunition()
{
	_pBitFlag->OnFlag(FlagUseAmmunition);
}

void Flintlock::SettingMissFire(bool on)
{
	if (on)
		_pBitFlag->OnFlag(FlagMissFire);
	else
		_pBitFlag->OffFlag(FlagMissFire);
}

void Flintlock::OnFlashHolePowder()
{
	_pBitFlag->OnFlag(FlagFlashHolePowder);
}

void Flintlock::OnOverChargePowder()
{
	_pBitFlag->OnFlag(FlagOverChargePowder);
}


void Flintlock::OnMuzzlePowder()
{
	_pBitFlag->OnFlag(FlagMuzzlePowder);
}

void Flintlock::OnFire()
{
	_pBitFlag->OnFlag(FlagFire);
}

bool Flintlock::CheckAim()
{
	return _pBitFlag->CheckFlag(FlagAiming);
}

bool Flintlock::CheckTrigger()
{
	return _pBitFlag->CheckFlag(FlagTrigger);
}

bool Flintlock::CheckStateFlag(Flag flag)
{
	return _pBitFlag->CheckFlag(flag);
}

bool Flintlock::Initialize(const Vector3& position)
{
	SetName("Flintlock");
	_currentState = (int)ReloadAndFireStep::Puor;
	_currentPivot = (int)FlintlockPivot::Middle;

	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pTimer = AddComponent<Engine::Timer>("Timer");

	_pTimer->AddTimer(TimerEnd);

	_vecTextures.push_back(Engine::FindTexture(L"Musket_Flintlock"));

	_pCollider = AddCollider("Muzzle");
	_pCollider->InitalizeOffset(Vector3(280.f, -113.f, 0.f));
	_pCollider->SetScale(Vector3(25.f, 25.f, 0.f));

	_pCollider = AddCollider("RammerSlot");
	_pCollider->InitalizeOffset(Vector3(250.f, -85.f, 0.f));
	_pCollider->SetScale(Vector3(25.f, 25.f, 0.f));

	_pCollider = AddCollider("CockPinPosition");
	_pCollider->InitalizeOffset(Vector3(-115.f, 5.f, 0.f));
	_pCollider->SetScale(Vector3(5.f, 5.f, 0.f));
	_pCollider->SetActive(false);

	_pCollider = AddCollider("FlashHolePosition");
	_pCollider->InitalizeOffset(Vector3(-85.f, -15.f, 0.f));
	_pCollider->SetScale(Vector3(5.f, 5.f, 0.f));
	_pCollider->SetActive(false);

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket",
		Rammer::Create(_pTransform->GetPosition(), this));

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket",
		CockPin::Create(this));

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket",
		FlashHole::Create(dynamic_cast<MusketObject*>(this)));

	_pRammer = dynamic_cast<Rammer*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Rammer"));
	_pSelectedMouse = dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));
	_pBubble = dynamic_cast<Bubble*>(Engine::MyGetObject((int)LayerGroup::UI, "Section", "Bubble"));
	_pCockPin = dynamic_cast<CockPin*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "CockPin"));

	return true;
}

void Flintlock::Free()
{
}

Flintlock* Flintlock::Create(const Vector3& position)
{
	Flintlock* pInstance = new Flintlock;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
