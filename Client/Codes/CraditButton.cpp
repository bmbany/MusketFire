#include "CraditButton.h"

#include "Client_Define.h"
#include "BitFlag.h"
#include "Letterhead.h"

int CraditButton::Update(const float& deltaTime)
{
	if (_pBitFlag->CheckFlag(FlagActivate))
	{
		if (Engine::IsKeyDown(Input::DIM_LB))
		{
			Engine::AddObjectInLayer((int)LayerGroup::UI, "UI",
				Letterhead::Create(Vector3(640.f, 360.f, -100.f), 10, nullptr));
			Engine::MyPlaySound("SFX_SFX_LetterOpen", (int)SoundGroup::SFX);
		}
	}
	return 0;
}

int CraditButton::LateUpdate(const float& deltaTime)
{
	return 0;
}

void CraditButton::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void CraditButton::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void CraditButton::OnCollision(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pBitFlag->OnFlag(FlagActivate);
		_index = 1;
	}
}

void CraditButton::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pBitFlag->OffFlag(FlagActivate);
	}
	_index = 0;
}

bool CraditButton::Initialize()
{
	_pTransform->SetPosition(Vector3(1150.f, 650.f, -2.f));

	_vecTextures.push_back(Engine::FindTexture(L"UI_Button_cradit"));
	
	Engine::Collider* pCollider = AddCollider("ControlsButton");
	pCollider->SetScale(Vector3(180.f, 60.f, 0.f));

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	return true;
}

CraditButton* CraditButton::Create()
{
	CraditButton* pInstance = new CraditButton;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}