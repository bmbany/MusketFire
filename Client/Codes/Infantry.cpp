//#include "Infantry.h"
//#include "cstdlib"
//
//#include "Animation.h"
//#include "Timer.h"
//#include "Client_Define.h"
//
//int Infantry::Update(const float& deltaTime)
//{   
//    if (_pTimer->IsOverTime(0, randomNumber))
//    {
//        ChangeState(0);
//        ChangeState(1);
//        ChangeState(2);
//        _pTimer->SetActive(1, true);
//        _pTimer->SetActive(0, false);
//    }
//    if (_pTimer->IsOverTime(1, 0.5f))
//    {
//        ChangeState(0);
//        ChangeState(1);
//        ChangeState(2);
//        _pTimer->SetActive(2, true);
//        _pTimer->SetActive(1, false);
//    }
//    if (_pTimer->IsOverTime(2, 0.5f))
//    {
//        ChangeState(0);
//        _pTimer->SetActive(3, true);
//        _pTimer->SetActive(2, false);
//    }
//    if (_pTimer->IsOverTime(3, 0.5f))
//    {
//        ChangeState(1);
//        _pTimer->SetActive(4, true);
//        _pTimer->SetActive(3, false);
//    }
//    if (_pTimer->IsOverTime(4, 0.5f))
//    {
//        ChangeState(2);
//        _pTimer->SetActive(0, true);
//        _pTimer->SetActive(4, false);
//        SetRandomNumber(randomNumber);
//    }
//
//    return 0;
//}
//
//int Infantry::LateUpdate(const float& deltaTime)
//{
//    return 0;
//}
//
//void Infantry::Render(Gdiplus::Graphics* pGraphics)
//{
//    _pSpriteRenderer->SetDrawInformation(_pAnimation->GetCurrentFrame(), Engine::CameraPosition());
//    _pSpriteRenderer->Draw(pGraphics, _pAnimation->GetCurrentFrame());
//}
//
//void Infantry::AddRenderer()
//{
//    Engine::AddRenderGroup((int)RenderGroup::Object, this);
//}
//
//bool Infantry::Initialize(const Vector3& position)
//{
//    _pTransform->SetPosition(position);
//
//    _vecTextures.resize(Animation_End);
//    _vecTextures[Ani_Idle] = Engine::FindTexture(L"Soldier_Enemy_idle");
//    _vecTextures[Ani_Fire] = Engine::FindTexture(L"Soldier_Enemy_fire");
//    _vecTextures[Ani_Aim] = Engine::FindTexture(L"Soldier_Enemy_aimstart");
//
//    _pTimer = AddComponent<Engine::Timer>("Timer");
//    _pTimer->AddTimer(5);
//    _pTimer->SetActive(0, true);
//    SetRandomNumber(randomNumber);
//
//    _pAnimation = AddComponent<Engine::Animation>("Animation");
//    _pAnimation->Initialize(Animation_End);
//
//    int size = _vecTextures[Ani_Idle]->GetImageCount();
//    for (int i = 0; i < size; i++)
//    {
//        _pAnimation->AddFrame(Ani_Idle,
//            _vecTextures[Ani_Idle]->GetImage(i, false),
//            _vecTextures[Ani_Idle]->GetImage(i, true), size / 0.5f);
//    }
//
//    size = _vecTextures[Ani_Aim]->GetImageCount();
//    for (int i = 0; i < size; i++)
//    {
//        _pAnimation->AddFrame(Ani_Aim,
//            _vecTextures[Ani_Aim]->GetImage(i, false),
//            _vecTextures[Ani_Aim]->GetImage(i, true), size / 0.1f);
//    }
//
//    size = _vecTextures[Ani_Fire]->GetImageCount();
//    for (int i = 0; i < size; i++)
//    {
//        _pAnimation->AddFrame(Ani_Fire,
//            _vecTextures[Ani_Fire]->GetImage(i, false),
//            _vecTextures[Ani_Fire]->GetImage(i, true), size / 0.2f);
//    }
//
//    _pAnimation->ChangeAnimation(Ani_Idle);
//    _pAnimation->FlipImage(true);
//
//    return true;
//}
//
//Infantry* Infantry::Create(const Vector3& position)
//{
//    Infantry* pInstance = new Infantry;
//    pInstance->Initialize(position);
//    return pInstance;
//}
//
//void Infantry::ChangeState(int a)
//{
//    if (Infantry_State[a] == Idle)
//    {
//        std::cout << a <<"Infantry Idle" << std::endl;
//        Infantry_State[a] = Aim;
//        if (IsHit==true)
//        {
//            Infantry_State[a] = Death;
//        }
//        return;
//    }
//
//    if (Infantry_State[a] == Aim)
//    {
//        std::cout <<a<< "Infantry Aim" << std::endl;
//        Infantry_State[a] = Attack;
//        if (IsHit == true)
//        {
//            Infantry_State[a] = Death;
//        }
//        return;
//    }
//
//    if (Infantry_State[a] == Attack)
//    {
//        std::cout <<a<< "Infantry Attack" << std::endl;
//        Infantry_State[a] = Idle;
//        if (IsHit == true)
//        {
//            Infantry_State[a] = Death;
//        }
//        return;
//    }
//}
//
//int Infantry::SetRandomNumber(float& number)
//{
//    while (number < 15)
//    {
//        number = ((rand() % 20) + 1);
//    }
//    return number;
//}
