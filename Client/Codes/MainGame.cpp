#include "MainGame.h"

#ifdef __MINJAE__
//#include "TestStage_1.h"
#include "BattleField.h"
#endif

#ifdef __YOUNGUNG__
//#include "TestStage_2.h"
#endif

#ifdef __GANGJIN__
//#include "TestStage_3.h"
#endif

#include "Intro.h"
#include "Title.h"
#include "Item.h"
#include "GameManager.h"
#include "Client_Define.h"

bool MainGame::Run()
{
	int isEvent = 0;
	_pGameManager->FixedUpdateGame(50);
	isEvent = _pGameManager->UpdateGame();
	_pGameManager->LateUpdateGame();
	_pGameManager->RenderGame();

	if (-5 == isEvent)
		return false;

	return true;
}

bool MainGame::Initialize(HINSTANCE hInstance, HWND hWnd, HDC hdc)
{
	Engine::GameManager::GameDefaultSetting info;
	info.hInstance = hInstance;
	info.hWnd = hWnd;
	info.hdc = hdc;
	info.layerSize = (int)LayerGroup::End;
	info.maxSoundGroup = (int)SoundGroup::End;
	info.renderGroupSize = (int)RenderGroup::End;
	info.width = WINCX;
	info.height = WINCY;

	_pGameManager = Engine::GameManager::GetInstance();
	_pGameManager->Initialize(info);

	// Texture
	_pGameManager->LoadTexture(L"../Resources/Texture");

	// Sound
	_pGameManager->LoadSound("../Resources/Sound");
	_pGameManager->SetMasterVolume(0.7f);
	_pGameManager->SetPitch((int)SoundGroup::EnemyVoice, 0.95f);
	_pGameManager->SetVolume((int)SoundGroup::EnemyVoice, 0.75f);
	_pGameManager->SetPitch((int)SoundGroup::PlayerVoice, 0.88f);
	_pGameManager->SetVolume((int)SoundGroup::PlayerVoice, 0.8f);

	// Render Sort
	_pGameManager->SetSortGroup((int)RenderGroup::Item, [](Engine::GameObject* pLhs, Engine::GameObject* pRhs)->bool
		{
			return static_cast<Item*>(pLhs)->GetPriority() < static_cast<Item*>(pRhs)->GetPriority();
		});

	_pGameManager->SetSortGroup((int)RenderGroup::Player, [](Engine::GameObject* pLhs, Engine::GameObject* pRhs)->bool
		{
			return pLhs->GetTransform()->GetPosition().x > pRhs->GetTransform()->GetPosition().x;
		});

	_pGameManager->SetSortGroup((int)RenderGroup::Section, [](Engine::GameObject* pLhs, Engine::GameObject* pRhs)->bool
		{
			return pLhs->GetTransform()->GetPosition().z > pRhs->GetTransform()->GetPosition().z;
		});

	_pGameManager->SetSortGroup((int)RenderGroup::UI, [](Engine::GameObject* pLhs, Engine::GameObject* pRhs)->bool
		{
			return pLhs->GetTransform()->GetPosition().z > pRhs->GetTransform()->GetPosition().z;
		});

	//_pGameManager->ChagneScene(TestStage::Create());
	_pGameManager->ChagneScene(Intro::Create());

	return true;
}

void MainGame::Free()
{
	Engine::SafeRelease(_pGameManager);
}

MainGame* MainGame::Create(HINSTANCE hInstance, HWND hWnd, HDC hdc)
{
	MainGame* pInstance = new MainGame;
	if (pInstance->Initialize(hInstance, hWnd, hdc))
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
