#include "Frame.h"
#include "BitmapRenderer.h"
#include "Client_Define.h"

int Frame::Update(const float& deltaTime)
{
    return 0;
}

int Frame::LateUpdate(const float& deltaTime)
{
    return 0;
}

void Frame::Render(Gdiplus::Graphics* pGraphics)
{
    _pBitmapRenderer->SetDrawInformation();
    _pBitmapRenderer->Draw();
}

void Frame::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::Section, this);
}

bool Frame::Initialize()
{
    // Transform
    _pTransform->SetPosition(Vector3(640.f, 508.f, -1.f));

    // Texture
    _vecTextures.push_back(Engine::FindTexture(L"UI_Frame"));

    // Component
    _pBitmapRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");
    _pBitmapRenderer->Initialize((*_vecTextures[0])[1], Engine::GetOriginHDC(), Engine::GetBackHDC(), _pTransform, true);

    return true;
}

Frame* Frame::Create()
{
    Frame* pInstance = new Frame;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
