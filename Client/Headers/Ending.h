#pragma once

#include "Scene.h"
class Ending : public Engine::Scene
{
private:
	explicit Ending() = default;
	virtual ~Ending() = default;

public:
	// Scene��(��) ���� ��ӵ�
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	bool Initialize() override;

public:
	static Ending* Create();

private:
	void Free() override;

private:
	HWND _videoHWND;
};

