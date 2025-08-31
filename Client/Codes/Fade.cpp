#include "Fade.h"
#include "Client_Define.h"

int Fade::Update(const float& deltaTime)
{
    _elapsed += deltaTime;

    switch (_fadeInfo.option)
    {
    case Fade::Fade_In:        
        _fadeInfo.alpha -= 255.f * deltaTime * _fadeInfo.speed;
        if (0.f >= _fadeInfo.alpha) _fadeInfo.alpha = 0.f;
        break;

    case Fade::Fade_Out:        
        _fadeInfo.alpha += 255.f * deltaTime * _fadeInfo.speed;
        if (255.f <= _fadeInfo.alpha) _fadeInfo.alpha = 255.f;
        break;

    case Fade::Fade_OutIn:
        if (_elapsed >= _fadeInfo.delay)
        {
            if (!_isOutIn)
            {
                _fadeInfo.alpha += 255.f * deltaTime * _fadeInfo.speed;
                if (255.f <= _fadeInfo.alpha)
                {
                    _fadeInfo.alpha = 255.f;
                    _elapsed = _fadeInfo.delay;
                    _isOutIn = true;
                }
            }
            else
            {
                if (0.5f <= _elapsed - _fadeInfo.delay)
                {
                    _fadeInfo.alpha -= 255.f * deltaTime * _fadeInfo.speed;
                    if (0.f >= _fadeInfo.alpha)
                    {
                        _fadeInfo.alpha = 0.f;
                        SetDead();
                    }
                }
            }
        }
        break;
    }

    return 0;
}

int Fade::LateUpdate(const float& fDeltaTime)
{
    if (_elapsed >= _fadeInfo.life)
        SetDead();

    return 0;
}

void Fade::Render(Gdiplus::Graphics* pGraphics)
{
    Gdiplus::Rect rc;
    rc.X = 0;
    rc.Y = 0;
    rc.Width = WINCX;
    rc.Height = WINCY;

    Gdiplus::Color color((BYTE)_fadeInfo.alpha, (BYTE)_fadeInfo.color, (BYTE)_fadeInfo.color, (BYTE)_fadeInfo.color);
    Gdiplus::SolidBrush solideBrush(color);
    pGraphics->FillRectangle(&solideBrush, rc);
}

void Fade::AddRenderer()
{
    Engine::AddRenderGroup((int)RenderGroup::Fade, this);
}

bool Fade::Initialize(const FadeInfo& info)
{
    _fadeInfo = info;

    switch (_fadeInfo.option)
    {
    case Fade::Fade_In:
        _fadeInfo.alpha = 255.f;
        break;
    case Fade::Fade_OutIn:
    case Fade::Fade_Out:
        _fadeInfo.alpha = 0.f;
        break;
    }

    return true;
}

void Fade::Free()
{
}

Fade* Fade::Create(const FadeInfo& info)
{
    Fade* pInstance = new Fade;
    if (pInstance->Initialize(info))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
