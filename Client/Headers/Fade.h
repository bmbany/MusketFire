#pragma once
#include "GameObject.h"

class Fade : public Engine::GameObject
{
public:
	enum Fade_Option { Fade_In, Fade_Out, Fade_OutIn };
	struct FadeInfo
	{
		Fade_Option option;
		float		speed;
		float		alpha;
		float		life;
		float		delay = 0.f;
		float		color = 0.f;
	};

private:
	explicit Fade() = default;
	virtual ~Fade() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize(const FadeInfo& info);
	void Free() override;

public:
	static Fade* Create(const FadeInfo& info);

private:
	FadeInfo _fadeInfo;
	float	 _elapsed = 0.f;
	bool	 _isOutIn = false;
};

