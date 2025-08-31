#pragma once
#include "GameObject.h"

namespace Engine
{
	class Animation;
}
enum class RenderGroup;
class Effect : public Engine::GameObject
{
public:
	struct EffectInfo
	{
		Vector3 position;
		Vector3 direction;
		Engine::Transform* pTarget = nullptr;
		const wchar_t* textureTag = nullptr;
		RenderGroup renderGroup;
		float life = 1.f;
		float speed = 0.f;
		float aniSpeed = 0.f;
		int fixFrame = 0;
		bool isFlip = false;
		bool isActiveLife = false;
		bool isFixFrame = false;
		bool isFalling = false;
		bool isDecelerate = false;
		bool isFadeOut = false;
	};

private:
	explicit Effect() = default;
	virtual ~Effect() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize(const EffectInfo& info);
	void Free() override;

public:
	static Effect* Create(const EffectInfo& info);

private:
	EffectInfo			_info;
	Engine::Animation*	_pAnimation = nullptr;
	Engine::Transform*	_pTarget = nullptr;
	float				_originSpeed = 0.f;
	float				_elapsed = 0.f;
};

