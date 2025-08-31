#include "BackGround.h"

#include "BitmapRenderer.h"
#include "Client_Define.h"


int BackGround::Update(const float& deltaTime)
{
    return 0;
}

int BackGround::LateUpdate(const float& deltaTime)
{
    return 0;
}

void BackGround::Render(Gdiplus::Graphics* pGraphics)
{
    _pBitmamRenderer->SetDrawInformation();
    _pBitmamRenderer->Draw();
}

void BackGround::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::BackGround, this);
}

bool BackGround::Initialize(const wchar_t* textureTag)
{
    _vecTextures.push_back(Engine::FindTexture(textureTag));

    // Component
    _pBitmamRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");    
    _pBitmamRenderer->Initialize((*_vecTextures[0])[0], Engine::GetOriginHDC(), Engine::GetBackHDC(), _pTransform);
        
    return true;
}

BackGround* BackGround::Create(const wchar_t* textureTag)
{
    BackGround* pInstance = new BackGround;    
    if (pInstance->Initialize(textureTag))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
