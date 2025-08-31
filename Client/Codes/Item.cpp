#include "Item.h"
#include "Mouse.h"
#include "BitFlag.h"

void Item::SettingGravity(const float& deltaTime)
{
	const float ScreenHeight = 720.f;
	const float CombatZoneTop = 0.f;
	const float CombatZoneBottom = 320.f;
	const float CombatZoneGround = 255.f;

	if (_pItemFlag->CheckFlag(ItemFlag::FlagInInventory))
	{
		_currentGravity = 0.f;
	}
	else
	{
		_currentGravity = _gravity;
	}

	//std::cout << _pSelectedMouse->GetMousePosition().x << " " << _pSelectedMouse->GetMousePosition().y << std::endl;

	if (ScreenHeight > _pTransform->GetPosition().y && CombatZoneBottom <= _pTransform->GetPosition().y)
	{
		_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x,
			_pTransform->GetPosition().y + _currentGravity * deltaTime, 0.f));
		if (_oldScale.x != 0 || _oldScale.y != 0)
		{
			_pTransform->SetScale(_oldScale);
		}
		else
		{
			_pTransform->SetScale(Vector3(1.f, 1.f, 1.f));
		}
		_pItemFlag->OffFlag(ItemFlag::FlagBattleField);
	}
	else if (ScreenHeight <= _pTransform->GetPosition().y ||
		CombatZoneTop <= _pTransform->GetPosition().y ||
		CombatZoneBottom > _pTransform->GetPosition().y)
	{
		if (_pTransform->GetPosition().y <= CombatZoneBottom)
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x,
				CombatZoneGround, 0.f));
		}
		else
		{
			_pTransform->SetPosition(Vector3(_pTransform->GetPosition().x * 0.5f,
				CombatZoneGround, 0.f));

		}
		_pTransform->SetScale(_settingBattleFieldScale);
		_pItemFlag->OnFlag(ItemFlag::FlagBattleField);
	}
}

void Item::InteractionMouse()
{
	if (_pItemFlag->CheckFlag(ItemFlag::FlagActivate))
	{
		if (Engine::IsKeyPress(Input::DIM_LB) && _pSelectedMouse->IsSelectedObject())
		{
			if (_pSelectedMouse->GetSelectedObjectID() == _thisColliderID)
			{

				_priority = _pSelectedMouse->GetPriority();
				if (Engine::IsKeyDown(Input::DIM_LB))
				{
					_mousePosition = _pSelectedMouse->GetMousePosition();
					_offset = _pTransform->GetPosition() - _mousePosition;

				}

				_newPosition = _pSelectedMouse->GetMousePosition() + _offset;
				_pTransform->SetPosition(_newPosition);
			}

			_pSelectedMouse->GetCollider("Mouse")->SetActive(false);

		}
	}
	else if (!Engine::IsKeyPress(Input::DIM_LB) && Engine::IsKeyUp(Input::DIM_LB))
	{
		_pSelectedMouse->GetCollider("Mouse")->SetActive(true);
	}
}

void Item::RenderRimLight(Gdiplus::Graphics* pGraphics)
{
	if (_pItemFlag->CheckFlag(ItemFlag::FlagBattleField))
	{
			//std::cout << "in7" << std::endl;
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[1]);
		Vector3 cameraPosition = Engine::CameraPosition();
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[1]);
	}
}
