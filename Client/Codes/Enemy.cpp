//#include "Enemy.h"
//#include "Timer.h"
//#include "Animation.h"
//
//#include "Client_Define.h"
//
//Enemy* Enemy::Create()
//{
//	Enemy* pInstance = new Enemy;
//	if (pInstance->Initialize())
//		return pInstance;
//
//	Engine::SafeRelease(pInstance);
//	return nullptr;
//}
//bool Enemy::Initialize()
//{
//	// �̸� ����
//	SetName("Flintlock");
//
//	// Transform ����
//	_pTransform->SetScale(Vector3(1.f, 1.f, 0.f));
//	_pTransform->SetPosition(Vector3(600.f, 540.f, 0.f));
//	
//	// Component �߰� (����� �� ������)
//	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");
//
//	// Texture �⺻ ����
//	// ex Enemy\slime\0.bmp -> �±� Enemy_slime
//	// Engine::FindTexture(L"���ҽ� �±�");
//	_vecTextures.push_back(Engine::FindTexture(L"Flintlock"));
//	// (*_vecTextures[�ε���])[���� �ҷ��� �̹��� �ε���]
//
//
//	 Collider �⺻ ���� �浹�� ��ü�� �߰�
//	_pCollider = AddCollider("Muzzle");
//	_pCollider->SetOffset(Vector3(320.f, -20.f, 0.f));
//	_pCollider->SetScale(Vector3(15.f, 15.f, 0.f));
//	_pCollider->SetActive(true);	
//
//	// SpriteRenderer �⺻ ���� bmp���Ͽ��� ������ �÷� ��
//	_pSpriteRenderer->SetColorKey(Gdiplus::Color(255, 0, 204), Gdiplus::Color(255, 0, 255));
//	_pAnimation = AddComponent<Engine::Animation>("Animation");	
//	_pTimer = AddComponent<Engine::Timer>("Timer");
//	_pTimer->AddTimer(2);
//	_pTimer->SetActive(0, true);
//	_pTimer->SetActive(1, false);
//	
//	_Infantry = Infantry::Create();
//
//	return true;
//}
//
//int Enemy::Update(const float& fDeltaTIme)
//{	
//	switch (enemyState)
//	{
//	case Idle:
//		if (_pTimer->IsOverTime(0, 2.f))
//		{
//			std::cout << "Enemy Fire" << std::endl;
//			enemyState = Attack;
//			_pTimer->SetActive(0, false);
//			_pTimer->SetActive(1, true);
//			_Infantry->SetActive(false);
//
//		}
//		break;
//
//	case Attack:
//		if (_pTimer->IsOverTime(1, 2.f))
//		{
//			std::cout << "Enemy Idle" << std::endl;
//			enemyState = Idle;
//			_pTimer->SetActive(1, false);
//			_pTimer->SetActive(0, true);
//			_Infantry->SetActive(true);
//		}
//		break;
//
//	case Death:
//		break;
//	}
//
//	return 0;
//}
//int Enemy::LateUpdate(const float& fDeltaTime)
//{
//	return 0;
//}
//void Enemy::Render(Gdiplus::Graphics* pGraphics)
//{
//	_pSpriteRenderer->SetDrawInformation((*_vecTextures[0])[0], Engine::CameraPosition());
//	_pSpriteRenderer->Draw(pGraphics, (*_vecTextures[0])[0]);
//}
//
//void Enemy::AddRenderer()
//{
//	Engine::AddRenderGroup((int)RenderGroup::Object, this);
//}
//
//ScareCrow* ScareCrow::Create()
//{
//	ScareCrow* pInstance = new ScareCrow;
//	if (pInstance->ScareCrow::Initialize())
//		return pInstance;
//
//	Engine::SafeRelease(pInstance);
//	return nullptr;
//}
//bool ScareCrow::Initialize()
//{
//	_pTimer = AddComponent<Engine::Timer>("Timer");
//	_pTimer->AddTimer(2);
//	_pTimer->SetActive(0, true);
//	_pTimer->SetActive(1, false);
//	
//	return true;
//}
//int ScareCrow::Update(const float& fDeltaTime)
//{
//	switch (ScareCrowState)
//	{
//	case Idle:
//		if (Enemy::_pTimer->IsOverTime(0, 2.f))
//		{
//			std::cout << "ScareCrow Idle" << std::endl;
//			Enemy::_pTimer->SetActive(0, false);
//			Enemy::_pTimer->SetActive(1, true);
//		}
//
//		if (_pTimer->IsOverTime(1, 2.f))
//		{
//			std::cout << "ScareCrow Idle" << std::endl;
//			Enemy::_pTimer->SetActive(1, false);
//			Enemy::_pTimer->SetActive(0, true);
//		}
//
//		break;
//
//	case Death:
//		break;
//	}
//	return 0;
//}
//
//int ScareCrow::LateUpdate(const float& fDeltaTime)
//{
//	return 0;
//}
//
//void ScareCrow::Render(Gdiplus::Graphics* pGraphics)
//{
//
//}
//
//void ScareCrow::AddRenderer()
//{
//
//}
//
//
//
//Infantry* Infantry::Create()
//{
//	Infantry* pInstance = new Infantry;
//	if (pInstance->Infantry::Initialize())
//		return pInstance;
//
//	Engine::SafeRelease(pInstance);
//	return nullptr;
//}
//bool Infantry::Initialize()
//{
//	_pTimer = AddComponent<Engine::Timer>("Timer");
//	_pTimer->AddTimer(3);
//	_pTimer->SetActive(0, true);
//	_pTimer->SetActive(1, false);
//	_pTimer->SetActive(2, false);
//	return true;
//}
//int Infantry::Update(const float& fDeltaTime)
//{
//	switch (Infantry_State)
//	{
//	case Idle:
//		if (_pTimer->IsOverTime(0, 15.f))
//		{
//			std::cout << "Infantry Fire" << std::endl;
//			Infantry_State = Aim;
//			Enemy::_pTimer->SetActive(0, false);
//			Enemy::_pTimer->SetActive(1, true);
//		}
//		break;
//
//	case Aim :
//		if (Enemy::_pTimer->IsOverTime(2, 1.f))
//		{
//			std::cout << "Infantry Idle" << std::endl;
//			Infantry_State = Attack;
//			Enemy::_pTimer->SetActive(1, false);
//			Enemy::_pTimer->SetActive(2, true);
//		}
//		break;
//
//	case Attack:
//		if (Enemy::_pTimer->IsOverTime(1, 1.f))
//		{
//			std::cout << "Infantry Idle" << std::endl;
//			Infantry_State = Idle;
//			Enemy::_pTimer->SetActive(2, false);
//			Enemy::_pTimer->SetActive(0, true);
//		}
//		break;
//
//	case Death:
//
//		break;
//	}
//	return 0;
//}
//
//int Infantry::LateUpdate(const float& fDeltaTime)
//{
//	return 0;
//}
//
//void Infantry::Render(Gdiplus::Graphics* pGraphics)
//{
//
//}
//
//void Infantry::AddRenderer()
//{
//	
//}
//
//bool Canon::Initialize()
//{
//	_pTimer = AddComponent<Engine::Timer>("Timer");
//	_pTimer->AddTimer(2);
//	_pTimer->SetActive(0, true);
//	_pTimer->SetActive(1, false);
//	return true;
//}
//
//Canon* Canon::Create()
//{
//	Canon* pInstance = new Canon;
//	if (pInstance->Canon::Initialize())
//		return pInstance;
//
//	Engine::SafeRelease(pInstance);
//	return nullptr;
//}
//
//int Canon::Update(const float& fDeltaTime)
//{
//	switch (Canon_State)
//	{
//	case Idle:
//		if (_pTimer->IsOverTime(0, 3.f))
//		{
//			std::cout << "Canon Idle" << std::endl;
//			Canon_State = Attack;
//			_pTimer->SetActive(0, false);
//			_pTimer->SetActive(1, true);
//		}
//		break;
//
//	case Attack:
//		if (_pTimer->IsOverTime(1, 2.f))
//		{
//			std::cout << "Canon Fire" << std::endl;
//			Canon_State = Idle;
//			_pTimer->SetActive(1, false);
//			_pTimer->SetActive(0, true);
//		}
//		break;
//	case Death:
//		
//		break;
//	}
//	return 0;
//}
//
//int Canon::LateUpdate(const float& fDeltaTime)
//{
//	return 0;
//}
//
//void Canon::Render(Gdiplus::Graphics* pGraphics)
//{
//
//}
//
//void Canon::AddRenderer()
//{
//
//}
//
//int Drum::Update(const float& fDeltaTime)
//{
//
//}