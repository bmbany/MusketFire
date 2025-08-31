#pragma once
#include "Army.h"
class Scarecrow : public Army
{
private:
	enum Animation { Idle, Death, Animation_End };
private:
	explicit Scarecrow() = default;
	virtual ~Scarecrow() = default;

public:
	// Army을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void SetStateDeath() override;

private:
	bool Initialize(const Vector3& position);

public:
	static Scarecrow* Create(const Vector3& position);

private:
	Animation _currAnimation = Animation_End;
};

