//#pragma once
//#include "GameObject.h"
//
//namespace Engine
//{
//	class BitFlag;
//	class Timer;
//	class Animation;
//}
//
//class Infantry : public Engine::GameObject
//{
//
//public:
//	bool IsHit = false;
//	float Enemy1FireCool = 15.f;
//	float EnemyFireDelay = 0.5f;
//	int Infantry_TargetRow =0;
//	int hitchance = 100;
//	int distance = 0;
//	int firecount[3] = {3, 3, 3};
//	float randomNumber;
//
//	// GameObject을(를) 통해 상속됨
//	int Update(const float& deltaTime) override;
//	int LateUpdate(const float& deltaTime) override;
//	void Render(Gdiplus::Graphics* pGraphics) override;
//	void AddRenderer() override;
//
//	Engine::Timer*		 _pTimer = nullptr;
//	Engine::Animation*	 _pAnimation = nullptr;
//
//	bool Initialize(const Vector3& position);
//	static Infantry* Create(const Vector3& position);
//	enum State { Idle, Aim, Attack, Death };
//	enum Animation { Ani_Idle, Ani_Aim, Ani_Fire, Animation_End };
//	State Infantry_State[3];
//
//	void ChangeState(int a);
//	void Battle(int a);
//	int SetRandomNumber(float& number);
//};
//
