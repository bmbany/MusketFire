#include "FrontGround.h"
#include "Client_Define.h"

int FrontGround::Update(const float& deltaTime)
{
    return 0;
}

int FrontGround::LateUpdate(const float& deltaTime)
{
    return 0;
}

void FrontGround::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void FrontGround::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::FrontGround, this);
}

bool FrontGround::Initialize(const wchar_t* textureTag, const Vector3& position)
{
    // Transform
    _pTransform->SetPosition(position);

    // Texture
    _vecTextures.push_back(Engine::FindTexture(textureTag));

    return true;
}

FrontGround* FrontGround::Create(const wchar_t* textureTag, const Vector3& position)
{
    FrontGround* pInstance = new FrontGround;
    if (pInstance->Initialize(textureTag, position))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
