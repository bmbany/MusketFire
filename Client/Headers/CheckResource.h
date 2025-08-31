#pragma once
#include "GameObject.h"
class CheckResource : public Engine::GameObject
{
private:
	explicit CheckResource() = default;
	virtual ~CheckResource() = default;

public:
	// GameObject��(��) ���� ��ӵ�
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

public:
	bool Initialize();
	static CheckResource* Create();

private:
	int _index = 0;
	int _maxIndex = 0;
};

