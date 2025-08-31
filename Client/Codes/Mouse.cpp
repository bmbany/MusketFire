#include "Mouse.h"
#include "Flintlock.h"
#include "BitFlag.h"
#include "Client_Define.h"
#include "Combat.h"
#include "TargetFrame.h"

int Mouse::Update(const float& deltaTime)
{
	ShowCursor(false);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(Engine::GetHWND(), &pt);

	_pTransform->SetPosition(Vector3((float)pt.x, (float)pt.y, -9999.f));

	if (Engine::IsKeyUp(Input::DIM_LB))
	{
		_pBitFlag->OnFlag(Flag::FlagLBUP);
		_pBitFlag->OffFlag(Flag::FlagLBDN);
		_pBitFlag->OffFlag(Flag::FlagLBPress);
	}
	if (Engine::IsKeyUp(Input::DIM_RB))
	{
		_pBitFlag->OnFlag(Flag::FlagRBUP);
		_pBitFlag->OffFlag(Flag::FlagLBDN);
		_pBitFlag->OffFlag(Flag::FlagRBPress);
	}
	if (Engine::IsKeyDown(Input::DIM_LB))
	{
		_pBitFlag->OnFlag(Flag::FlagLBDN);
		_pBitFlag->OffFlag(Flag::FlagLBUP);
	}
	if (Engine::IsKeyDown(Input::DIM_RB))
	{
		_pBitFlag->OnFlag(Flag::FlagRBDN);
		_pBitFlag->OffFlag(Flag::FlagRBUP);
	}
	if (Engine::IsKeyPress(Input::DIM_LB))
	{
		_pBitFlag->OnFlag(Flag::FlagLBPress);

	}
	if (Engine::IsKeyPress(Input::DIM_RB))
	{
		_pBitFlag->OnFlag(Flag::FlagRBPress);
	}
	return 0;
}

int Mouse::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Mouse::Render(Gdiplus::Graphics* pGraphics)
{
	//size
	const float ScreenHeight = 720.f;
	const float CombatZoneTop = 0.f;
	const float CombatZoneBottom = 300.f;
	//UI CrossHair
	const int crossHair = 0;
	const int crossHair_Grey = 0;
	const int crossHair_Black = 1;
	//UI Cursor
	const int cursor = 1;
	const int cursor_Choose = 0;
	const int cursor_Grab = 1;
	const int cursor_Normal = 2;
	
	
	if (Engine::IsKeyPress(Input::DIM_RB))
	{
		
		if (CombatZoneBottom > GetMousePosition().y)
		{
			if (nullptr == _pFlintlock || _pFlintlock->IsDead())
			{
				_pSpriteRenderer->SetDrawInformation((*_vecTextures[crossHair])[crossHair_Grey]);
				_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[crossHair])[crossHair_Grey]);
				return;
			}

			if (_pFlintlock->GetCurrentState() == (int)Flintlock::ReloadAndFireStep::Aim)
			{
				_pSpriteRenderer->SetDrawInformation((*_vecTextures[crossHair])[crossHair_Black]);
				_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[crossHair])[crossHair_Black]);
			}
			else
			{
				_pSpriteRenderer->SetDrawInformation((*_vecTextures[crossHair])[crossHair_Grey]);
				_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[crossHair])[crossHair_Grey]);
			}
		}
		else 
		{
			_pSpriteRenderer->SetDrawInformation((*_vecTextures[cursor])[cursor_Grab], Vector3(-12, -15, 0.f));
			_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[cursor])[cursor_Grab]);
		}
	}

	else if (Engine::IsKeyPress(Input::DIM_LB))
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[cursor])[cursor_Choose], Vector3(-12, -15, 0.f));
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[cursor])[cursor_Choose]);
	}
	else
	{
		_pSpriteRenderer->SetDrawInformation((*_vecTextures[cursor])[cursor_Normal], Vector3(-12, -15, 0.f));
		_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[cursor])[cursor_Normal]);
	}
	
}

void Mouse::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void Mouse::SelectObject(Engine::Collider* other)
{
	if (Engine::IsKeyPress(Input::DIM_LB))
	{
		_pSelectedObject = other->GetID();
		_pBitFlag->OnFlag(Flag::FlagSelect);
		_priority++;
	}
}

_uint Mouse::GetSelectedObjectID() const
{
	return _pSelectedObject;
}

_uint Mouse::GetPriority() const
{
	return _priority;
}

Vector3 Mouse::GetMousePosition()
{
	return _pTransform->GetPosition();
}

void Mouse::DisableSelectedObject()
{
	_pSelectedObject = 0;
	_pBitFlag->OffFlag(Flag::FlagSelect);
}

bool Mouse::IsMouseState(Flag flag)
{
	return _pBitFlag->CheckFlag(flag);
}

bool Mouse::IsSelectedObject()
{
	return _pBitFlag->CheckFlag(Flag::FlagSelect);
}

void Mouse::OnCollision(CollisionInfo info)
{
	if (*info.other->GetOwner() == "Flintlock")
	{
			_pFlintlock = static_cast<Flintlock*>(info.other->GetOwner());
		if (nullptr == _pFlintlock)
		{
		}
	}

	if (Engine::IsKeyPress(Input::DIM_RB))
	{
		if (*info.itSelf == "Mouse")
		{
			if (*info.other == "EnemyRow1")
			{
				Vector3 position = info.other->GetPosition();
				position.x = info.other->GetPosition().x + 325.f;
				position.y = info.other->GetPosition().y + 90.f;
				position.z = -1000.f;
				_pTargetFrame->SetActive(true);
				_pTargetFrame->SetPosition(position);
				//std::cout << "Active" << std::endl;
			}
			if (*info.other == "EnemyRow2")
			{
				Vector3 position = info.other->GetPosition();
				position.y = info.other->GetPosition().y + 90.f;
				position.z = -1000.f;
				_pTargetFrame->SetActive(true);
				_pTargetFrame->SetPosition(position);

			}
			if (*info.other == "EnemyRow3")
			{
				Vector3 position = info.other->GetPosition();
				position.y = info.other->GetPosition().y + 90.f;
				position.z = -1000.f;
				_pTargetFrame->SetActive(true);
				_pTargetFrame->SetPosition(position);

			}
		}
	}
	else
	{
		_pTargetFrame->SetActive(false);
	}

	if (*info.other == "GunPowderBody" ||
		*info.other == "Biscuit" ||
		*info.other == "Flint" ||
		*info.other == "LetterBody" ||
		*info.other == "RammerBody" ||
		*info.other == "Ammunition" ||
		*info.other == "PaperCartidgeBody")
	{
		if (Engine::IsKeyDown(Input::DIM_LB))
		{
			Engine::MyPlaySound("SFX_SFX_Grab", (int)SoundGroup::SFX);

			if (0 == _pSelectedObject)
				SelectObject(info.other);
		}
		else if (Engine::IsKeyUp(Input::DIM_LB))
		{
			Engine::MyPlaySound("SFX_SFX_Put", (int)SoundGroup::SFX);
			DisableSelectedObject();
		}
	}
}

void Mouse::OnCollisionExit(CollisionInfo info)
{
	_pBitFlag->OffFlag(Flag::FlagLockOn);
}

bool Mouse::Initialize()
{
	SetName("Mouse");
	Engine::Collider* pCollider = AddCollider("Mouse");

	//component
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	//texture
	_vecTextures.push_back(Engine::FindTexture(L"UI_CrossHair"));
	_vecTextures.push_back(Engine::FindTexture(L"UI_Cursor"));

	_pTargetFrame = TargetFrame::Create(Vector3(0.f,0.f,0.f));	
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pTargetFrame);
	
	//collider
	pCollider->SetScale(Vector3(10.f, 10.f, 0.f));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));
	return true;
}

Mouse* Mouse::Create()
{
	Mouse* pInstance = new Mouse;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}

void Mouse::Free()
{

}

void Mouse::ChangeFlag(enum Flag flag)
{
	_pBitFlag->OnFlag(flag);
}
