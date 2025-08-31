#include "Stamp.h"
#include "Client_Define.h"

int Stamp::Update(const float& deltaTime)
{
    return 0;
}

int Stamp::LateUpdate(const float& deltaTime)
{
    return 0;
}

void Stamp::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[1], Engine::CameraPosition());
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[1]);
}

void Stamp::AddRenderer()
{
    if (IsActive())
        Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

bool Stamp::Initialize()
{
    _pTransform->SetPosition(Vector3(500.f, 540.f, -200.f));
    _vecTextures.push_back(Engine::FindTexture(L"UI_RestartButton"));

    return true;
}

Stamp* Stamp::Create()
{
    Stamp* pInstance = new Stamp;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
