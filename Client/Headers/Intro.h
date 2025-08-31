#pragma once

#include "Scene.h"
class Intro : public Engine::Scene
{
private:
	explicit Intro() = default;
	virtual ~Intro() = default;

public:
	// Scene을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	bool Initialize() override;

public:
	static Intro* Create();

private:
	void Free() override;

private:
	HWND _videoHWND;
};

