#include "NextButton.h"
#include "Client_Define.h"

int NextButton::Update(const float& deltaTime)
{
    return 0;
}

int NextButton::LateUpdate(const float& deltaTime)
{
    return 0;
}

void NextButton::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0]);
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
}

void NextButton::AddRenderer()
{
    if (!_isBattleEnd)
        return;

    Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void NextButton::OnCollision(CollisionInfo info)
{
    if (!_isBattleEnd)
        return;

    if (*info.other != "Mouse")
        return;

    if (Engine::IsKeyDown(Input::DIM_LB))
        SetActive(true);
}

void NextButton::OnCollisionExit(CollisionInfo info)
{
    SetActive(false);
}

bool NextButton::Initialize()
{
    SetName("NextButton");
    SetActive(false);

    // Transform
    _pTransform->SetPosition(Vector3(1100.f, 100.f, 0.f));

    // Component
    _vecTextures.push_back(Engine::FindTexture(L"UI_Button_next"));

    // Collider
    Engine::Collider* pCollider = AddCollider("NextButton");
    pCollider->SetScale(Vector3((float)(*_vecTextures[0])[0]->GetWidth(), (float)(*_vecTextures[0])[0]->GetHeight(), 0.f));

    return true;
}

NextButton* NextButton::Create()
{
    NextButton* pInstance = new NextButton;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
