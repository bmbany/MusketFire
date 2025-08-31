#include "ControlsButton.h"

#include "Client_Define.h"
#include "BitFlag.h"
#include "Letterhead.h"

int ControlsButton::Update(const float& deltaTime)
{
	if (_pBitFlag->CheckFlag(FlagActivate))
	{
		if (Engine::IsKeyDown(Input::DIM_LB))
		{
			Engine::AddObjectInLayer((int)LayerGroup::UI, "UI",
				Letterhead::Create(Vector3(640.f, 360.f, -100.f), 9, nullptr));
			Engine::MyPlaySound("SFX_SFX_LetterOpen", (int)SoundGroup::SFX);
		}
	}
    return 0;
}

int ControlsButton::LateUpdate(const float& deltaTime)
{
    return 0;
}

void ControlsButton::Render(Gdiplus::Graphics* pGraphics)
{
	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void ControlsButton::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void ControlsButton::OnCollision(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pBitFlag->OnFlag(FlagActivate);
		_index = 1;
	}
}

void ControlsButton::OnCollisionExit(CollisionInfo info)
{
	if (*info.other == "Mouse")
	{
		_pBitFlag->OffFlag(FlagActivate);
	}
	_index = 0;
}

bool ControlsButton::Initialize()
{
	_pTransform->SetPosition(Vector3(640.f, 525.f, -2.f));

	_vecTextures.push_back(Engine::FindTexture(L"UI_Button_controls"));

	Engine::Collider* pCollider = AddCollider("ControlsButton");
	pCollider->SetScale(Vector3(180.f, 60.f, 0.f));

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

    return true;
}

ControlsButton* ControlsButton::Create()
{
	ControlsButton* pInstance = new ControlsButton;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
    return nullptr;
}
