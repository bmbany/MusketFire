#include "Soldier.h"
#include "Effect.h"

#include "Timer.h"
#include "Animation.h"
#include "BitFlag.h"
#include "Client_Define.h"

int Soldier::Update(const float& deltaTime)
{
    _deltaTime = deltaTime;

    if (_pBitFlag->CheckFlag(FlagStart))
    {
        _pTransform->AddPosition(_pTransform->GetDirection() * _speed * deltaTime);
        float distance = (_startPosition - _pTransform->GetPosition()).Length();
        
        if (5.f >= distance)
        {
            _pBitFlag->OffFlag(FlagStart);
            ChangeAnimation(Idle);
        }

        return 0;
    }

    switch (_currState)
    {
    case Soldier::State_Idle:
        StateIdle();
        break;
    case Soldier::State_Walk:
        StateWalk();
        break;
    case Soldier::State_Aim:
        StateAim();
        break;
    case Soldier::State_Fire:
        StateFire();
        break;
    case Soldier::State_Death:
        StateDeath();
        break;
    case Soldier::State_Charge:
        StateCharge();
        break;
    }

    return 0;
}

int Soldier::LateUpdate(const float& deltaTime)
{
    _deltaTime = deltaTime;

    switch (_currAnimation)
    {
    case Soldier::Idle:
        if (_pTimer->IsOverTime(Smoke, 0.5f))
        {
            _pTimer->SetActive(Smoke, false);

            Effect::EffectInfo info;
            info.renderGroup = RenderGroup::Effect;
            info.position = _pTransform->GetPosition();
            float x = 70.f;
            if (Enemy == _type)
            {
                x *= -1.f;
                info.isFlip = true;
            }
            info.position.x += x;
            info.position.y -= 23.f;
            info.textureTag = L"Effect_MuzzleFire_musket_smoke";
            info.aniSpeed = 0.2f;

            Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
        }
        break;

    case Soldier::Walk:
        if (_pTimer->IsOverTime(SceneChange, 2.f))
        {
            _pBitFlag->OffFlag(FlagMove);
            _pTimer->SetActive(SceneChange, false);
            ChangeAnimation(Idle);
            _pTransform->SetPosition(_startPosition);
        }
        _pTimer->SetActive(WalkSound, true);
        if (_pTimer->IsOverTime(WalkSound, 0.45f))
        {
            _pTimer->SetElapsed(WalkSound, 0.f);
            Engine::MyPlaySound("SFX_SFX_Walking", (int)SoundGroup::Soldier);
        }
        break;

    case Soldier::AimStart:        
        if (_pAnimation->IsBetweenFrame(12, 14))
            _pAnimation->SetFrame(12);

        break;

    case Soldier::Fire:
        if (_pAnimation->IsLastFrame())
        {
            _pBitFlag->OffFlag(FlagFire);
            _pTimer->SetActive(Smoke, true);
            ChangeAnimation(Idle);
        }
        break;

    case Soldier::Death:
        if (_pAnimation->IsBetweenFrame(13, 15))
            _pAnimation->SetFrame(13);

        break;
    case Soldier::Stab:
        if (_pAnimation->IsLastFrame())
        {
            _pAnimation->ChangeAnimation(Idle);
        }
    }

    return 0;
}

void Soldier::Render(Gdiplus::Graphics* pGraphics)
{
    _pSpriteRenderer->SetDrawInformation(_pAnimation->GetCurrentFrame(), Engine::CameraPosition());
    _pSpriteRenderer->Draw(pGraphics, _pAnimation->GetCurrentFrame());
}

void Soldier::AddRenderer()
{
    switch (_type)
    {
    case Soldier::Player:
        Engine::AddRenderGroup((int)RenderGroup::Player, this);
        break;
    case Soldier::Enemy:
        Engine::AddRenderGroup((int)RenderGroup::Enemy, this);
        break;
    }    
}

void Soldier::ChangeAnimation(Animation animation)
{
    if (_pBitFlag->CheckFlag(FlagFire | FlagMove | FlagDeath | FlagStart))
        return;

    if (_pAnimation->ChangeAnimation(animation))
    {
        _currAnimation = animation;

        switch (animation)
        {
        case Soldier::Idle:
            _currState = State_Idle;
            break;
        case Soldier::Walk:
            _currState = State_Walk;
            break;
        case Soldier::AimStart:
            _currState = State_Aim;
            break;
        case Soldier::Fire:
            _currState = State_Fire;
            break;
        case Soldier::Death:
            _currState = State_Death;
            break;
        }        
    }
}

void Soldier::ChargeAttack()
{
    if (Death == _currState || !_isChargeInfantry)
        return;

    _currState = State_Charge;
    _pBitFlag->OnFlag(FlagCharge);
    ChangeAnimation(ChargeWalk);
}

void Soldier::OnCollisionEnter(CollisionInfo info)
{
    if (_isStateDeath)
        return;

    if (_pBitFlag->CheckFlag(FlagCharge))
    {
        if (*info.other == "Soldier")
        {
            Soldier* pItSelf = static_cast<Soldier*>(info.itSelf->GetOwner());

            if (Player == pItSelf->_type)
                return;

            Soldier* pTarget = static_cast<Soldier*>(info.other->GetOwner());

            if (Player == pTarget->_type)
            {
                if (Soldier::Death != pTarget->GetCurrAnimation())
                {                    
                    ChangeAnimation(Stab);
                }
            }
        }
    }
}

void Soldier::OnCollision(CollisionInfo info)
{
    if (_isStateDeath)
        return;

    if (_pBitFlag->CheckFlag(FlagCharge))
    {
        if (*info.other == "Soldier")
        {
            Soldier* pItSelf = static_cast<Soldier*>(info.itSelf->GetOwner());

            if (Player == pItSelf->_type)
                return;

            Soldier* pTarget = static_cast<Soldier*>(info.other->GetOwner());

            if (Player == pTarget->_type)
            {
                if (Soldier::Death != pTarget->GetCurrAnimation())
                {
                    if (_pAnimation->IsBetweenFrame(6, 7))
                    {
                        pTarget->SetStateDeath();
                        _pBitFlag->OffFlag(FlagCharge);
                    }
                }
            }
        }        
    }
}

void Soldier::SetStateDeath()
{
    if (_pBitFlag->CheckFlag(FlagStart))
        return;

    _pBitFlag->SetFlag(0);
    _isStateDeath = true;
    ChangeAnimation(Death);
    _currState = State_Death;
    Engine::MyPlaySound("SFX_SFX_MusketHit", (int)SoundGroup::SFX);
}

void Soldier::SetStartPosition()
{
    _pTransform->SetPosition(_startPosition);
}

bool Soldier::IsChargetAttack()
{   
    float distance = (_startPosition - _pTransform->GetPosition()).Length();

    if (10.f <= distance)
        return true;

    return false;
}

void Soldier::StateIdle()
{
    //_startPosition = _pTransform->GetPosition();
}

void Soldier::StateWalk()
{    
    _pBitFlag->OnFlag(FlagMove);
    _pTimer->SetActive(SceneChange, true);
    _pTransform->AddPosition(Vector3(1.f, 0.f, 0.f) * _speed * _deltaTime);
}

void Soldier::StateAim()
{
    ChangeAnimation(AimStart);
}

void Soldier::StateFire()
{
    if (!_pBitFlag->CheckFlag(FlagFire))
    {
        Effect::EffectInfo info;
        info.renderGroup = RenderGroup::Effect;
        info.position = _pTransform->GetPosition();
        float x = 90.f;
        if (Enemy == _type)
        {
            x *= -1.f;
            info.isFlip = true;
        }
        info.position.x += x;
        info.position.y -= 44.f;
        info.textureTag = L"Effect_MuzzleFire_musket";
        info.aniSpeed = 0.1f;

        Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
        Engine::MyPlaySound("SFX_SFX_Fire", (int)SoundGroup::SFX);
        _pBitFlag->OnFlag(FlagFire);        
    }
}

void Soldier::StateDeath()
{
    _pBitFlag->OnFlag(FlagDeath);
}

void Soldier::StateCharge()
{
    if (_pBitFlag->CheckFlag(FlagCharge))
    {
        if (ChargeWalk == _currAnimation)
        {
            _pAnimation->ChangeAnimation(ChargeWalk);
            _pTransform->AddPosition(Vector3(-1.f, 0.f, 0.f) * 100.f * _deltaTime);
        }
    }
}

bool Soldier::Initialize(const Vector3& position, Type type, bool isCharge)
{    
    _type = type;
    _startPosition = position;
    _pTransform->SetPosition(position);
    _isChargeInfantry = isCharge;    

    if (SpawnSoldier == type)
        _type = Player;

    // Collider
    Engine::Collider* pColldier = AddCollider("Soldier");
    pColldier->SetScale(Vector3(50.f, 100.f, 0.f));

    // Component
    _pAnimation = AddComponent<Engine::Animation>("Animation");
    _pAnimation->Initialize(Animation_End);

    _pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

    _pTimer = AddComponent<Engine::Timer>("Timer");
    _pTimer->AddTimer(Timer_End);

    // Texture
    _vecTextures.resize(Animation_End);

    switch (_type)
    {
    case Soldier::Player:        
        _vecTextures[Idle] = Engine::FindTexture(L"Soldier_Player_idle");
        _vecTextures[Walk] = Engine::FindTexture(L"Soldier_Player_walk");
        _vecTextures[Fire] = Engine::FindTexture(L"Soldier_Player_fire");
        _vecTextures[AimStart] = Engine::FindTexture(L"Soldier_Player_aimstart");
        _vecTextures[Death] = Engine::FindTexture(L"Soldier_Player_death");
        break;

    case Soldier::Enemy:
        _vecTextures[Idle] = Engine::FindTexture(L"Soldier_Enemy_idle");
        _vecTextures[Walk] = Engine::FindTexture(L"Soldier_Enemy_walk");
        _vecTextures[Fire] = Engine::FindTexture(L"Soldier_Enemy_fire");
        _vecTextures[AimStart] = Engine::FindTexture(L"Soldier_Enemy_aimstart");
        _vecTextures[Death] = Engine::FindTexture(L"Soldier_Enemy_death");
        _vecTextures[ChargeWalk] = Engine::FindTexture(L"Soldier_Enemy_walk_knife");
        _vecTextures[Stab] = Engine::FindTexture(L"Soldier_Enemy_stab");
        break;
    }

    if (Enemy == _type)
        _pAnimation->FlipImage(true);

    // Animation
    int size = _vecTextures[Idle]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Idle,
            _vecTextures[Idle]->GetImage(i, false),
            _vecTextures[Idle]->GetImage(i, true), 0.5f / size);
    }

    size = _vecTextures[Walk]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Walk,
            _vecTextures[Walk]->GetImage(i, false),
            _vecTextures[Walk]->GetImage(i, true), 0.4f / size);
    }    

    size = _vecTextures[AimStart]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(AimStart,
            _vecTextures[AimStart]->GetImage(i, false),
            _vecTextures[AimStart]->GetImage(i, true), 0.1f / size);
    }

    size = _vecTextures[Fire]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Fire,
            _vecTextures[Fire]->GetImage(i, false),
            _vecTextures[Fire]->GetImage(i, true), 0.2f / size);
    }

    size = _vecTextures[Death]->GetImageCount();
    for (int i = 0; i < size; i++)
    {
        _pAnimation->AddFrame(Death,
            _vecTextures[Death]->GetImage(i, false),
            _vecTextures[Death]->GetImage(i, true), 0.1f / size);
    }

    if (Enemy == _type)
    {
        size = _vecTextures[ChargeWalk]->GetImageCount();
        for (int i = 0; i < size; i++)
        {
            _pAnimation->AddFrame(ChargeWalk,
                _vecTextures[ChargeWalk]->GetImage(i, false),
                _vecTextures[ChargeWalk]->GetImage(i, true), 0.4f / size);
        }

        size = _vecTextures[Stab]->GetImageCount();
        for (int i = 0; i < size; i++)
        {
            _pAnimation->AddFrame(Stab,
                _vecTextures[Stab]->GetImage(i, false),
                _vecTextures[Stab]->GetImage(i, true), 0.3f / size);
        }
    }
    _pAnimation->ChangeAnimation(Idle);
    _currAnimation = Idle;

    _speed = 100.f;

    if (Enemy == _type)
    {
        Vector3 currPosition = { position.x + 150.f, position.y, 0.f };
        _pTransform->SetPosition(currPosition);
        _pTransform->SetDirection((_startPosition - currPosition).Normalized());
        ChangeAnimation(Walk);
        _pBitFlag->OnFlag(FlagStart);
    }

    if (SpawnSoldier == type)
    {
        _pTransform->SetPosition(Vector3(-70.f, 200.f, 0.f));
        _pTransform->SetDirection(Vector3(1.f, 0.f, 0.f));
        ChangeAnimation(Walk);
        _pBitFlag->OnFlag(FlagStart);
    }

    return true;
}

Soldier* Soldier::Create(const Vector3& position, Type type, bool isCharge)
{
    Soldier* pInstance = new Soldier;
    if (pInstance->Initialize(position, type, isCharge))
        return pInstance;

    Engine::SafeRelease(pInstance);
    return nullptr;
}
