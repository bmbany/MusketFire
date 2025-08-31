#include "GunPowder.h"
#include "Item.h"
#include "CockPin.h"
#include "FlashHole.h"
#include "Flintlock.h"
#include "MiniGame.h"
#include "BitFlag.h"
#include "Client_Define.h"
#include "Animation.h"
#include "Effect.h"
#include "Timer.h"

#include <strsafe.h>

int GunPowder::Update(const float& deltaTime)
{
	SettingGravity(deltaTime);

	InteractionMouse();

	if (_pItemFlag->CheckFlag(FlagActivate) &&
		Engine::IsKeyPress(Input::DIM_RB) &&
		0 <= _currentStack)
	{
		if (0.8f < (*_pTimer)[Sound].elapsed)
		{
			Engine::MyPlaySound("SFX_SFX_Powder", (int)SoundGroup::SFX);
			(*_pTimer)[Sound].elapsed = 0.f;
		}
	}

	if (0.1f <= (*_pTimer)[Effect].elapsed)
	{
		_currentFrameIndex++;
		(*_pTimer)[Effect].elapsed = 0.f;
	}

	if (_currentFrameIndex >= _maxFrame)
	{
		_isLastFrame = true;
		_currentFrameIndex = 0;
	}

	return 0;
}

int GunPowder::LateUpdate(const float& deltaTime)
{
	if (0 >= _currentStack && nullptr == _pMiniGame)
	{
		SetDead();
	}

	if (_pBitFlag->CheckFlag(FlagCorrectUse) &&
		_pItemFlag->CheckFlag(FlagActivate) &&
		Engine::IsKeyDown(Input::DIM_RB) &&
		!IsDead())
	{
		CreateMiniGame(deltaTime);
		--_currentStack;
	}
	else
	{
		_pBitFlag->ResetFlag();
	}

	UpdateMiniGame();

	return 0;
}

void GunPowder::Render(Gdiplus::Graphics* pGraphics)
{
	RenderRimLight(pGraphics);

	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);

	if (_pItemFlag->CheckFlag(FlagActivate) &&
		Engine::IsKeyPress(Input::DIM_RB) && 0 <= _currentStack)
	{


		int offsetWidth = static_cast<int>(_pTransform->GetScale().x * ((*_vecTextures[1])[0]->GetWidth() >> 1));
		int offsetHeight = static_cast<int>(_pTransform->GetScale().y * ((*_vecTextures[1])[0]->GetHeight() >> 1));

		_drawRect.X = _pTransform->GetPosition().x + _animationOffset.x - offsetWidth;
		_drawRect.Y = _pTransform->GetPosition().y + _animationOffset.y - offsetHeight;
		_drawRect.Width = offsetWidth << 1;
		_drawRect.Height = offsetHeight << 1;

		pGraphics->DrawImage((*_vecTextures[1])[_currentFrameIndex], _drawRect);
	}

	if (!_pItemFlag->CheckFlag(ItemFlag::FlagBattleField))
	{
		const int bufferSize = 256;
		WCHAR buffer[bufferSize];
		int bufferSizeInBytes = bufferSize * sizeof(wchar_t);

		LPCTSTR format = L"%d / 16";

		StringCbPrintfW(buffer, bufferSizeInBytes, format, _currentStack);

		DrawString(pGraphics,
			Gdiplus::Color(0, 0, 0),
			_pTransform->GetPosition().x + 20.f,
			_pTransform->GetPosition().y + 30.f,
			buffer);
	}

}

void GunPowder::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Item, this);
}

void GunPowder::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Inventory" && *info.itSelf == "GunPowderBody")
	{
		_pItemFlag->OnFlag(FlagInInventory);
	}
	if (*info.other == "Musket" && *info.itSelf == "GunPowderBody")
	{
		_pItemFlag->OffFlag(FlagInInventory);
	}
	if (*info.other == "FlashHole" && *info.itSelf == "GunPowderSupplies")
	{
		if (nullptr == _pFlashHole)
		{
			_pFlashHole = dynamic_cast<FlashHole*>(info.other->GetOwner());
		}

		_currentGaugeScale = 5.f;
		_pBitFlag->OnFlag(FlagCorrectUse);
	}
	else if (*info.other == "Muzzle" && *info.itSelf == "GunPowderSupplies")
	{
		if (nullptr == _pFlintlock)
		{
			_pFlintlock = dynamic_cast<Flintlock*>(info.other->GetOwner());
		}

		if (_pFlintlock->GetCurrentPivot() == (int)Flintlock::FlintlockPivot::Up)
		{
			_currentGaugeScale = 4.f;
			_pBitFlag->OnFlag(FlagCorrectUse);
		}
	}

	if (*info.other == "Mouse" && *info.itSelf == "GunPowderBody")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}
}

void GunPowder::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse" && *info.itSelf == "GunPowderBody")
	{
		_pItemFlag->OffFlag(FlagActivate);
	}
}

void GunPowder::StateCharge()
{
}

bool GunPowder::IsSuccess() const
{
	return _pBitFlag->CheckFlag(FlagSuccess);
}

bool GunPowder::IsFail() const
{
	return _pBitFlag->CheckFlag(FlagFail);
}

bool GunPowder::IsOverCharge() const
{
	return _pBitFlag->CheckFlag(FlagOverCharge);
}

void GunPowder::DrawString(Gdiplus::Graphics* pGraphics,
	Gdiplus::Color color, float x, float y, const WCHAR* string)
{
	Gdiplus::PrivateFontCollection fontCollection;
	Gdiplus::FontFamily fontFamily;
	fontCollection.AddFontFile(L"../Resources/Font/EFKratos.ttf");

	int fontFamilyCount = fontCollection.GetFamilyCount();
	if (fontFamilyCount <= 0)
	{
		std::wcerr << L"Failed to load font file" << std::endl;
		return;
	}

	int numFound = 0;
	fontCollection.GetFamilies(1, &fontFamily, &numFound);
	if (numFound <= 0)
	{
		std::wcerr << L"Failed to get font family" << std::endl;
		return;
	}

	Gdiplus::Font font(&fontFamily, 10);
	Gdiplus::SolidBrush brush(color);
	Gdiplus::PointF pointF(x, y);

	INT len = (INT)wcslen(string);

	pGraphics->DrawString(string, len, &font, pointF, &brush);
}

bool GunPowder::Initialize(const Vector3& position)
{
	SetName("GunPowder");
	_pTransform->SetScale(Vector3(0.8f, 0.8f, 0.f));
	_oldScale = _pTransform->GetScale();

	_settingBattleFieldScale = Vector3(0.5f, 0.5f, 0.f);

	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));

	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_vecTextures.push_back(Engine::FindTexture(L"Item_GunPowder"));
	_vecTextures.push_back(Engine::FindTexture(L"Effect_GunPowder_Pour"));

	_pItemFlag = AddComponent<Engine::BitFlag>("BitFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pCollider = AddCollider("GunPowderBody");
	_thisColliderID = _pCollider->GetID();
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(80.f, 60.f, 0.f));

	_pCollider = AddCollider("GunPowderSupplies");
	_pCollider->InitalizeOffset(Vector3(-105.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(50.f, 130.f, 0.f));

	_animationOffset = Vector3(-95.f, 28.f, 0.f);

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(End);
	(*_pTimer)[Sound].isActive = true;
	(*_pTimer)[Effect].isActive = true;

	return true;
}

void GunPowder::Free()
{
}

void GunPowder::CreateMiniGame(const float& deltaTime)
{
	_pMiniGame = MiniGame::Create(_currentGaugeScale);
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pMiniGame);
}

void GunPowder::UpdateMiniGame()
{
	if (nullptr == _pMiniGame)
		return;

	if (_pMiniGame->IsSuccess())
	{
		_pBitFlag->OnFlag(FlagSuccess);
		_pBitFlag->OffFlag(FlagFail | FlagOverCharge);
		Engine::MyPlaySound("SFX_SFX_PowderFin", (int)SoundGroup::SFX);
		_pMiniGame = nullptr;
	}
	else if (_pMiniGame->IsFail())
	{
		_pBitFlag->OnFlag(FlagFail);
		_pBitFlag->OffFlag(FlagSuccess | FlagOverCharge);
		Engine::MyPlaySound("SFX_SFX_PowderNone", (int)SoundGroup::SFX);
		_pMiniGame = nullptr;
	}
	else if (_pMiniGame->IsOverCharge())
	{
		_pBitFlag->OnFlag(FlagOverCharge);
		_pBitFlag->OffFlag(FlagSuccess | FlagFail);
		Engine::MyPlaySound("SFX_SFX_PowderFin", (int)SoundGroup::SFX);
		_pMiniGame = nullptr;
	}

}

GunPowder* GunPowder::Create(const Vector3& position)
{
	GunPowder* pInstance = new GunPowder;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
