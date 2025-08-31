#include "Canon.h"
#include "Effect.h"

#include "Timer.h"
#include "BitmapRenderer.h"
#include "Client_Define.h"

int Canon::Update(const float& deltaTime)
{
	if (_pTimer->IsOverTime(Smoke, 0.5f))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::Effect;
		info.textureTag = L"Effect_MuzzleFire_canon_smoke";
		info.aniSpeed = 0.1f;
		info.isFlip = true;
		info.position = _pTransform->GetPosition();
		info.position.x -= 90.f;
		info.position.y -= 33.f;

		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetActive(Smoke, false);
	}

	return 0;
}

int Canon::LateUpdate(const float& deltaTime)
{
	return 0;
}

void Canon::Render(Gdiplus::Graphics* pGraphics)
{
	_pBitmapRenderer->SetDrawInformation(Engine::CameraPosition());
	_pBitmapRenderer->Draw();
}

void Canon::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Enemy, this);
}

void Canon::SetStateDeath()
{
	_isStateDeath = true;
}

void Canon::Attack()
{
	Effect::EffectInfo info;
	info.renderGroup = RenderGroup::Effect;
	info.textureTag = L"Effect_MuzzleFire_canon";
	info.aniSpeed = 0.1f;
	info.isFlip = true;
	info.position = _pTransform->GetPosition();
	info.position.x -= 90.f;
	info.position.y -= 33.f;

	Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
	_pTimer->SetActive(Smoke, true);
	Engine::CameraShake(0.5f, 10.f);

	Engine::MyPlaySound("SFX_SFX_CannonFire", (int)SoundGroup::SFX);
}

bool Canon::Initialize(const Vector3& position)
{
	_pTransform->SetPosition(position);

	// Texture
	_vecTextures.push_back(Engine::FindTexture(L"Canon"));

	// Component
	_pBitmapRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");
	_pBitmapRenderer->Initialize(_vecTextures[0]->GetImage(0, true), Engine::GetOriginHDC(), Engine::GetBackHDC(), _pTransform, true);

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(Timer_End);

	return true;
}

Canon* Canon::Create(const Vector3& position)
{
	Canon* pInstance = new Canon;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}

//#include "Timer.h"
//#include "Client_Define.h"
//#include "cstdlib"
//
//int Canon::Update(const float& deltaTime)
//{
//	 if (_pTimer->IsOverTime(0, randomNumber))
//    {
//        ChangeState(0);
//        ChangeState(1);
//        ChangeState(2);
//        _pTimer->SetActive(0, false);
//        _pTimer->SetActive(1, true);
//    }
//    if (_pTimer->IsOverTime(1, 0.5f))
//    {
//        ChangeState(0);
//        ChangeState(1);
//        ChangeState(2);
//        _pTimer->SetActive(1, false);
//        _pTimer->SetActive(2, true);
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
//
//    }
//	return 0;
//}
//
//int Canon::LateUpdate(const float& deltaTime)
//{
//	return 0;
//}
//
//void Canon::Render(Gdiplus::Graphics* pGraphics)
//{
//}
//
//void Canon::AddRenderer()
//{
//}
//
//bool Canon::Initialize()
//{
//	_pTimer = AddComponent<Engine::Timer>("Timer");
//	_pTimer->AddTimer(5);
//	_pTimer->SetActive(0, true);
//	SetRandomNumber(randomNumber);
//	return true;
//}
//
//Canon* Canon::Create()
//{
//	Canon* pInstance = new Canon;
//	pInstance->Initialize();
//	return pInstance;
//}
//
//float Canon::SetRandomNumber(int& number)
//{
//    while (number < 26)
//    {
//        number = ((rand() % 26) + 1);
//    }
//	return number;
//}
//
//void Canon::ChangeState(int number)
//{
//	if (Canon_State[number] == Idle)
//	{
//		std::cout << number<<"CanonIdle" << std::endl;
//		Canon_State[number] = Aim;
//        return;
//	}
//	
//	if (Canon_State[number] == Aim)
//	{
//		std::cout << number << "CanonAim" << std::endl;
//		Canon_State[number] = Attack;
//        return;
//    }
//	
//	if (Canon_State[number] == Attack)
//	{
//		std::cout << number << "CanonAttack" << std::endl;
//		Canon_State[number] = Idle;
//        return;
//    }
//
//}
//
