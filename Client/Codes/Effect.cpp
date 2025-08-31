#include "Effect.h"
#include "Animation.h"

#include "Client_Define.h"

int Effect::Update(const float& deltaTime)
{
    float offset = 0.f;
    _elapsed += deltaTime;

    if (_info.isFadeOut)
    {
        offset = 1.f;
        if (_info.life <= _elapsed + offset)
            _pSpriteRenderer->SetColorMatrix(3, 3, -deltaTime / offset);
    }

    if (_info.pTarget)
    {
        _pTransform->SetPosition(_info.pTarget->GetPosition());
        _pTransform->AddPosition(_info.position);
    }
    else
    {
        if (_info.life >= _elapsed - offset)
        {
            if (_info.isDecelerate)
                _info.speed -= _originSpeed * deltaTime / _info.life;

            if (_info.isFalling)
            {
                _pTransform->AddPosition(Vector3(0.f, 1.f, 0.f) * deltaTime * (_originSpeed - _info.speed));
            }

            _pTransform->AddPosition(_pTransform->GetDirection() * _info.speed * deltaTime);
        }
    }

    return 0;
}

int Effect::LateUpdate(const float& deltaTime)
{
    if (_info.isActiveLife)
    {
        if (_info.life <= _elapsed)
            SetDead();
    }
    else
    {
        if (_pAnimation->IsLastFrame())
            SetDead();
    }    

    if (_info.isFixFrame)
        _pAnimation->SetFrame(_info.fixFrame);

    return 0;
}

void Effect::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation(_pAnimation->GetCurrentFrame(), Engine::CameraPosition());
    _pSpriteRenderer->Draw(pGraphics, _pAnimation->GetCurrentFrame());    
}

void Effect::AddRenderer()
{
    Engine::AddRenderGroup((int)_info.renderGroup, this);
}

bool Effect::Initialize(const EffectInfo& info)
{
    _info = info;
    SetName("Effect");

    // Transform
    _pTransform->SetPosition(_info.position);
    _pTransform->SetDirection(_info.direction);

    // Component
    _vecTextures.push_back(Engine::FindTexture(_info.textureTag));

    _pAnimation = AddComponent<Engine::Animation>("Animation");
    _pAnimation->Initialize(1);

    int size = _vecTextures[0]->GetImageCount();
    for (int i = 0; i < size; i++)
        _pAnimation->AddFrame(0, _vecTextures[0]->GetImage(i, false), _vecTextures[0]->GetImage(i, true), _info.aniSpeed / size);

    _pAnimation->FlipImage(_info.isFlip);

    _originSpeed = _info.speed;

    if (_info.isFixFrame)
        _pAnimation->SetFrame(_info.fixFrame);

    return true;
}

void Effect::Free()
{
}

Effect* Effect::Create(const EffectInfo& info)
{
    Effect* pInstance = new Effect;
    if (pInstance->Initialize(info))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
