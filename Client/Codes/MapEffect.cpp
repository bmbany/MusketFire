#include "MapEffect.h"
#include "Effect.h"
#include "Timer.h"
#include "Client_Define.h"
#include <cstdlib>

int MapEffect::Update(const float& deltaTime)
{
	_pTimer->SetActive(0, true);
	_pTimer->SetActive(1, true);
	_pTimer->SetActive(2, true);
	_pTimer->SetActive(3, true);

	if(_pTimer->IsOverTime(0, (float)(rand() % 20) + 2))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::Effect;
		info.textureTag = L"Effect_Smoke_A";
		info.aniSpeed = 1.f;
		info.position = { (float)(rand() % 1280), 170.f, 0.f };
		info.isFadeOut = true;
		info.isActiveLife = true;
		info.life = 2.f;

		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetElapsed(0, 0.f);
	}

	if (_pTimer->IsOverTime(1, (float)(rand() % 20) + 2))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::Effect;
		info.textureTag = L"Effect_Smoke_B";
		info.aniSpeed = 1.f;
		info.position = { (float)(rand() % 1280), 170.f, 0.f };
		info.isFadeOut = true;
		info.isActiveLife = true;
		info.life = 2.f;

		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetElapsed(1, 0.f);
	}

	if(_pTimer->IsOverTime(2, 5.f+(float)(rand() % 20) + 2))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::BackEffect;
		info.textureTag = L"Effect_Smoke_A";
		info.aniSpeed = 1.f;
		info.position = { (float)(rand() % 1280), 150.f, 0.f };
		info.isFadeOut = true;
		info.isActiveLife = true;
		info.life = 2.f;

		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetElapsed(2, 0.f);
	}
	
	if (_pTimer->IsOverTime(3, (float)(rand() % 20) + 2))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::BackEffect;
		info.textureTag = L"Effect_Smoke_B";
		info.aniSpeed = 1.f;
		info.position = { (float)(rand() % 1280), 150.f, 0.f };
		info.life = 2.f;
		info.isFadeOut = true;
		info.isActiveLife = true;

		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetElapsed(3, 0.f);
	}
	

	return 0;
}

int MapEffect::LateUpdate(const float& deltaTime)
{
	return 0;
}

void MapEffect::Render(Gdiplus::Graphics* pGraphics)
{
}

void MapEffect::AddRenderer()
{
	//Engine::AddRenderGroup((int)RenderGroup::Object, this);
}

bool MapEffect::Initialize(const Vector3& position)
{
	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(4);
	return true;
}

MapEffect* MapEffect::Create(const Vector3& position)
{
	MapEffect* pInstance = new MapEffect;
	if (pInstance->Initialize(position))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
