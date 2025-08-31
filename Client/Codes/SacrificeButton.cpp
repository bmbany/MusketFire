#include "SacrificeButton.h"

#include "BitmapRenderer.h"
#include "Client_Define.h"

int SacrificeButton::Update(const float& deltaTime)
{
    return 0;
}

int SacrificeButton::LateUpdate(const float& deltaTime)
{
    return 0;
}

void SacrificeButton::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void SacrificeButton::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void SacrificeButton::OnCollision(CollisionInfo info)
{
    if (*info.other == "Mouse")
    {
        _index = 1;

        if (Engine::IsKeyPress(Input::DIM_LB))
        {
            _index = 2;
        }

        if (Engine::IsKeyUp(Input::DIM_LB))
        {
            SetActive(true);
        }
    }
}

void SacrificeButton::OnCollisionExit(CollisionInfo info)
{
    _index = 0;
}

bool SacrificeButton::Initialize()
{
    SetName("Sacrifice");
    SetActive(false);

    // Transform
    _pTransform->SetPosition(Vector3(360.f, 350.f, 0.f));

    // Texture
    _vecTextures.push_back(Engine::FindTexture(L"UI_Button_Sacrifice"));

    // Collider
    Engine::Collider* pCollider = AddCollider("Sacrifice");
    pCollider->SetScale(Vector3(132.f, 62.f, 0.f));

    return true;
}

SacrificeButton* SacrificeButton::Create()
{
    SacrificeButton* pInstance = new SacrificeButton;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
