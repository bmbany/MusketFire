#pragma once
#include "Army.h"

namespace Engine 
{
	class Timer;
	class BitmapRenderer;
}
class Canon : public Army
{
private:
	enum Timer { Smoke, Timer_End };
private:
	explicit Canon() = default;
	virtual ~Canon() = default;

public:
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void SetStateDeath() override;

	void Attack();

private:
	bool Initialize(const Vector3& position);

public:
	static Canon* Create(const Vector3& position);

private:
	Engine::Timer*			_pTimer = nullptr;	
	Engine::BitmapRenderer* _pBitmapRenderer = nullptr;
};


//#pragma once
//#include "GameObject.h"
//
//namespace Engine 
//{
//	class BitFlag;
//	class Timer;
//	class Animation;
//}
//class Canon : public Engine::GameObject
//{
//public:
//	bool isHIt = false;
//	int randomNumber;
//
//	int Update(const float& deltaTime) override;
//	int LateUpdate(const float& deltaTime) override;
//	void Render(Gdiplus::Graphics* pGraphics) override;
//	void AddRenderer() override;
//	float SetRandomNumber(int& number);
//
//	Engine::Timer*			_pTimer = nullptr;
//	Engine::Animation*		_pAnimation = nullptr;
//
//	bool Initialize();
//	static Canon* Create();
//	enum State { Idle, Aim, Attack, Death };
//	State Canon_State[3];
//
//	void ChangeState(int number);
//};
//
