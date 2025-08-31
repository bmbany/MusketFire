#include "StartButton.h"

#include "Client_Define.h"

int StartButton::Update(const float& deltaTime)
{
    return 0;
}

int StartButton::LateUpdate(const float& deltaTime)
{
    return 0;
}

void StartButton::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void StartButton::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void StartButton::OnCollision(CollisionInfo info)
{
    if (*info.other == "Mouse")
    {
        _index = 1;
        if (Engine::IsKeyDown(Input::DIM_LB))
        {
            _isSceneChange = true;
        }
    }
}

void StartButton::OnCollisionExit(CollisionInfo info)
{
    _index = 0;
}

bool StartButton::Initialize()
{
    // Transform
    _pTransform->SetPosition(Vector3(640.f, 450.f, -2.f));

    // Texture
    _vecTextures.push_back(Engine::FindTexture(L"UI_Button_start"));

    // Collider
    Engine::Collider* pCollider = AddCollider("StartButton");
    pCollider->SetScale(Vector3(180.f, 60.f, 0.f));

    return true;
}

StartButton* StartButton::Create()
{
    StartButton* pInstance = new StartButton;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
