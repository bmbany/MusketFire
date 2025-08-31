#pragma once
#include "Section.h"

class MusketObject;
class Mouse;
class Musket : public Section
{
private:
	explicit Musket() = default;
	virtual ~Musket() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;

public:
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initailize();

public:
	static Musket* Create();

private:
	std::list<MusketObject*> _listMusket;
};

