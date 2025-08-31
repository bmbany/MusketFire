#include "Scarecrow.h"

#include "Animation.h"
#include "Client_Define.h"

int Scarecrow::Update(const float& deltaTime)
{    
    return 0;
}

int Scarecrow::LateUpdate(const float& deltaTime)
{
    if (Death == _currAnimation)
    {
        if (_pAnimation->IsBetweenFrame(8, 10))
            _pAnimation->SetFrame(8);
    }

    return 0;
}

void Scarecrow::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation(_pAnimation->GetCurrentFrame(), Engine::CameraPosition());
    _pSpriteRenderer->Draw(pGraphics, _pAnimation->GetCurrentFrame());
}

void Scarecrow::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::Enemy, this);
}

void Scarecrow::SetStateDeath()
{
    _isStateDeath = true;
    _pAnimation->ChangeAnimation(Death);
    _currAnimation = Death;
}

bool Scarecrow::Initialize(const Vector3& position)
{
    // Transform
    _pTransform->SetPosition(position);

    // Texture
    _vecTextures.resize(Animation_End);
    _vecTextures[Idle] = Engine::FindTexture(L"Scarecrow");
    _vecTextures[Death] = Engine::FindTexture(L"Scarecrow_death");

    // Component
    _pAnimation = AddComponent<Engine::Animation>("Animation");
    _pAnimation->Initialize(Animation_End);

    // Animation
    int size = _vecTextures[Idle]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Idle,
            _vecTextures[Idle]->GetImage(i, false),
            _vecTextures[Idle]->GetImage(i, true), size);
    }

    size = _vecTextures[Death]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Death,
            _vecTextures[Death]->GetImage(i, false),
            _vecTextures[Death]->GetImage(i, true), 0.3f / size);
    }

    _pAnimation->ChangeAnimation(Idle);
    _currAnimation = Idle;

    return true;
}

Scarecrow* Scarecrow::Create(const Vector3& position)
{
    Scarecrow* pInstance = new Scarecrow;

    if (pInstance->Initialize(position))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
