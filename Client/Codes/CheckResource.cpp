#include "CheckResource.h"
#include "Client_Define.h"

int CheckResource::Update(const float& deltaTime)
{
    if (Engine::IsMouseWheel(VK_MWHEELUP))
    {
        _index++;

        if (_maxIndex == _index) _index = 0;
    }

    if (Engine::IsMouseWheel(VK_MWHEELDOWN))
    {
        _index--;

        if (0 > _index) _index = _maxIndex - 1;
    }

    return 0;
}

int CheckResource::LateUpdate(const float& deltaTime)
{
    return 0;
}

void CheckResource::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[_index]);
    _pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[_index]);
}

void CheckResource::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

bool CheckResource::Initialize()
{
    _pTransform->SetPosition(Vector3(640.f, 360.f, 0.f));
    _vecTextures.push_back(Engine::FindTexture(L"Asset"));
    _maxIndex = _vecTextures[0]->GetImageCount();
    _pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));

    return true;
}

CheckResource* CheckResource::Create()
{
    CheckResource* pInstance = new CheckResource;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
