#include "ContinueButton.h"
#include "Stamp.h"
#include "Letterhead.h"

#include "Client_Define.h"

int ContinueButton::Update(const float& deltaTime)
{
    if (IsActive())
        _pLetterhead->SetActive(true);
    else
    {
        _pLetterhead->SetActive(false);
        _pStamp->SetActive(false);
    }

    return 0;
}

int ContinueButton::LateUpdate(const float& deltaTime)
{
    return 0;
}

void ContinueButton::Render(Gdiplus::Graphics* pGraphics)
{
}

void ContinueButton::AddRenderer()
{
    if (IsActive())
        Engine::AddRenderGroup((int)RenderGroup::UI, this);
}

void ContinueButton::OnCollision(CollisionInfo info)
{
    if (!IsActive())
        return;

    if (*info.itSelf == "Continue" && *info.other == "Mouse")
    {
        if (Engine::IsKeyDown(Input::DIM_LB))
        {
            Engine::MyPlaySound("SFX_SFX_Sealing", (int)SoundGroup::SFX);
            _pStamp->SetActive(true);
        }
        if (Engine::IsKeyUp(Input::DIM_LB))
        {
            _isRestart = true;
        }
    }
}

bool ContinueButton::Initialize()
{
    SetName("Continue");

    // Transform
    _pTransform->SetPosition(Vector3(640.f, 560.f, -120.f));

    // Texture
    _vecTextures.push_back(Engine::FindTexture(L"UI_Button"));

    // Colldier
    Engine::Collider* pCollider = AddCollider("Restart");
    pCollider->SetScale(Vector3(1280.f, 720.f, 0.f));
    pCollider->SetOffset(Vector3(0.f, -200.f, 0.f));

    pCollider = AddCollider("Continue");
    pCollider->SetScale(Vector3(300.f, 78.f, 0.f));
    pCollider->SetOffset(Vector3(0.f, -20.f, 0.f));

    _pLetterhead = Letterhead::Create(Vector3(640.f, 360.f, -110.f), 3, nullptr);
    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pLetterhead);
    _pLetterhead->SetActive(false);
    SetActive(false);

    _pStamp = Stamp::Create();
    Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pStamp);
    _pStamp->SetActive(false);

    return true;
}

ContinueButton* ContinueButton::Create()
{
    ContinueButton* pInstance = new ContinueButton;
    if (pInstance->Initialize())
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
