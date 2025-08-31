#include "Letter.h"
#include "Client_Define.h"
#include "BitFlag.h"
#include "Letterhead.h"

#include <string>

int Letter::Update(const float& deltaTime)
{
	SettingGravity(deltaTime);

	InteractionMouse();

	if (_pItemFlag->CheckFlag(FlagActivate) && !_pBitFlag->CheckFlag(FlagOpen))
	{
		if (Engine::IsKeyDown(Input::DIM_RB))
		{
			Engine::AddObjectInLayer((int)LayerGroup::UI, "Section",
				Letterhead::Create(Vector3(640.f, 360.f, -100.f), _thisLetterID, this));
			_pBitFlag->OnFlag(FlagOpen);
			Engine::MyPlaySound("SFX_SFX_LetterOpen", (int)SoundGroup::SFX);
		}
	}

	return 0;
}

int Letter::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Letter::Render(Gdiplus::Graphics* pGraphics)
{
	Vector3 cameraPosition = Engine::CameraPosition();

	RenderRimLight(pGraphics);

	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);

	/*for (auto& Collider : GetColliders())
		Collider->DrawCollider(pGraphics);*/
}

void Letter::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void Letter::OnCollision(CollisionInfo info)
{
	if (*info.other == "Restart")
		return;

	if (*info.other == "Inventory" && *info.itSelf == "LetterBody")
	{
		_pItemFlag->OnFlag(FlagInInventory);
	}
	if (*info.other == "Musket" && *info.itSelf == "LetterBody")
	{
		_pItemFlag->OffFlag(FlagInInventory);
	}

	if (*info.other == "Mouse" && *info.itSelf == "LetterBody")
	{
		_pItemFlag->OnFlag(FlagActivate);
	}
}

void Letter::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse" && *info.itSelf == "LetterBody")
	{
		_pItemFlag->OffFlag(FlagActivate);
	}
}

void Letter::SetClose()
{
	_pBitFlag->OffFlag(FlagOpen);
}

bool Letter::Initialize(const Vector3& position, int id)
{
	SetName("Letter");

	_pTransform->SetScale(Vector3(0.5f, 0.5f, 0.f));
	_oldScale = _pTransform->GetScale();

	_settingBattleFieldScale = Vector3(0.3f, 0.3f, 0.f);

	_pTransform->SetPosition(Vector3(position.x, position.y, position.z));

	_thisLetterID = id;

	_pItemFlag = AddComponent<Engine::BitFlag>("ItemFlag");
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_vecTextures.push_back(Engine::FindTexture(L"Item_Letter"));
	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));

	_pCollider = AddCollider("LetterBody");
	_pCollider->InitalizeOffset(Vector3(0.f, 0.f, 0.f));
	_pCollider->SetScale(Vector3(80.f, 50.f, 0.f));
	_thisColliderID = _pCollider->GetID();

	return true;
}

void Letter::Free()
{
}

Letter* Letter::Create(const Vector3& position, int id)
{
	Letter* pInstance = new Letter;
	if (pInstance->Initialize(position, id))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
