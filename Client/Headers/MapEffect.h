#pragma once
#include "GameObject.h"
namespace Engine
{
	class Timer;
}
class MapEffect : public Engine::GameObject
{
private:
	explicit MapEffect() = default;
	virtual ~MapEffect() = default;
public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

private:
	bool Initialize(const Vector3& position);

public:
	static MapEffect* Create(const Vector3& position);

private:
	Engine::Timer*	 _pTimer = nullptr;
};

