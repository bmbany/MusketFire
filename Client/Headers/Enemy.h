//#pragma once
//#include"GameObject.h"
//namespace Engine
//{
//	class BitFlag;
//	class Timer;
//	class Animation;
//}
//
//
//class Enemy : public Engine::GameObject
//{
//protected:
//	explicit Enemy() = default;
//	virtual ~Enemy() = default;
//
//public:
//	int Update(const float& fDeltaTime) override;
//	int LateUpdate(const float& fDeltaTime) override;
//	void Render(Gdiplus::Graphics* pGraphics) override;
//	void AddRenderer() override;
//	//void UpdateComponent(const float& deltaTime) override;
//	enum class Animation { Idle, Fire, Charge, End };
//
//	enum State { Idle, Aim, Attack, Charge, Death };
//
//protected:
//	bool Initialize();
//
//public:
//	static Enemy* Create();
//	State enemyState = Idle;
//
//protected:
//	Engine::Timer*		 _pTimer = nullptr;
//	Engine::Animation*	 _pAnimation = nullptr;
//public:
//	//ScareCrow* _pScareCrow = nullptr;
//	Infantry* _Infantry = nullptr;
//	//Canon* _pCanon = nullptr;
//};
//
//
//
////class ScareCrow : public Enemy
////{
////private :
////	explicit ScareCrow() = default;
////
////	virtual ~ScareCrow() = default;
////
////public:
////	int Update(const float& fDeltaTime) override;
////
////	int LateUpdate(const float& fDeltaTime) override;
////
////	void Render(Gdiplus::Graphics* pGraphics) override;
////
////	void AddRenderer() override;
////
////protected:
////	bool Initialize();
////public:
////	static ScareCrow* Create();
////	State ScareCrowState = Idle;
////
////public:
////	Infantry* _pInfantry = nullptr;
////	Canon* _pCanon = nullptr;	
////};
//
//
//
////class Infantry : public Enemy
////{
////private :
////	explicit Infantry() = default;
////
////	virtual ~Infantry() = default;
////
////public:
////	int Update(const float& fDeltaTime) override;
////
////	int LateUpdate(const float& fDeltaTime) override;
////
////	void Render(Gdiplus::Graphics* pGraphics) override;
////
////	void AddRenderer() override;
////
////protected:
////	bool Initialize();
////public : 
////	static Infantry* Create();
////	State Infantry_State = Idle;
////public:
//	//ScareCrow* _pScareCrow = nullptr;
//	//Canon* _pCanon = nullptr;
//
//};
//
//
////class Canon: public Enemy
////{
////private:
////	explicit Canon() = default;
////	
////	virtual ~Canon() = default;
////
////public:
////	int Update(const float& fDeltaTime) override;
////
////	int LateUpdate(const float& fDeltaTime) override;
////
////	void Render(Gdiplus::Graphics* pGraphics) override;
////
////	void AddRenderer() override;
////
////protected:
////	bool Initialize();
////public:
////	static Canon* Create();
////	State Canon_State = Idle;
////public:
////	ScareCrow* _pScareCrow = nullptr;
////	Infantry* _Infantry = nullptr;
////};
//
////class Drum : public Enemy
////{
////private:
////	explicit Drum() = default;
////	virtual ~Drum() = default;
////
////public:
////	int Update(const float& fDeltaTime) override;
////
////	int LateUpdate(const float& fDeltaTime) override;
////
////	void Render(Gdiplus::Graphics* pGraphics) override;
////
////	void AddRenderer() override;
////
////protected : 
////	bool Initialize();
////public:
////	static Drum* Create();
////	Drum Drum_State = Idle;
////public:
////	ScareCrow* _pScareCrow = nullptr;
////	Infantry* _Infantry = nullptr;
////	Canon* _pCanon = nullptr;
////};