#include "Combat.h"

#include <fstream>
#include <string>

#include "Mouse.h"
#include "NextButton.h"
#include "ContinueButton.h"
#include "SacrificeButton.h"

#include "UI.h"
#include "Fade.h"
#include "Effect.h"
#include "FrontGround.h"

#include "Bubble.h"
#include "PlayerStatus.h"
#include "Soldier.h"
#include "Canon.h"
#include "Scarecrow.h"
#include "EnemyAI.h"

// Item
#include "FireStepText.h"
#include "Flintlock.h"
#include "Rammer.h"
#include "Ammunition.h"
#include "GunPowder.h"
#include "PaperCartidge.h"
#include "Flint.h"
#include "Biscuit.h"
#include "Letter.h"
#include "Letterhead.h"
#include "CockPin.h"

#include "BitmapRenderer.h"
#include "Timer.h"
#include "BitFlag.h"
#include "Client_Define.h"

#include "Intro.h"

int Combat::Update(const float& deltaTime)
{
	// 시연회용 마스터 키
	if (Engine::IsKeyDown(DIK_HOME) || _isRePlay)
	{
		Engine::ChagneScene(Intro::Create());
		Engine::StopSound((int)SoundGroup::BGM);

		return 0;
	}
	if (Engine::IsKeyDown(DIK_INSERT))
	{
		_isInfinityFire = !_isInfinityFire;
	}

	if (Engine::IsKeyDown(DIK_END))
	{
		StartInventory();
	}

	if (IsActive())
	{
		InputEvent();
	}

	DebugEnemyCreate();
	Sacrifice();
	EnemyVolleyFire();
	VolleyFire();

	return 0;
}

int Combat::LateUpdate(const float& deltaTime)
{
	GameOver();
	GameClear();
	PlayBGM();

	return 0;
}

void Combat::Render(Gdiplus::Graphics* pGraphics)
{
	_pBitmapRenderer->SetDrawInformation(_stage, Engine::CameraPosition());
	_pBitmapRenderer->Draw(_stage);
}

void Combat::AddRenderer()
{
	Engine::AddRenderGroup((int)RenderGroup::Section, this);
}

void Combat::OnCollision(CollisionInfo info)
{
	Engine::GameObject* pObject = info.other->GetOwner();

	if (*pObject != "Mouse" && *info.other->GetOwner() != "Rammer")
	{
		_dropItems.insert(pObject);
	}

	if (*info.other == "Mouse")
		SetActive(true);
	else
		return;

	if (*info.itSelf == "EnemyRow1")
		_currInfo.row = 0;

	if (*info.itSelf == "EnemyRow2")
		_currInfo.row = 1;

	if (*info.itSelf == "EnemyRow3")
		_currInfo.row = 2;
	
	if (*info.itSelf == "RePlay")
	{
		if (Engine::IsKeyDown(Input::DIM_LB))
		{
			_isRePlay = true;
		}
	}
}

void Combat::OnCollisionExit(CollisionInfo info)
{
	Engine::GameObject* pObject = info.other->GetOwner();
	_dropItems.erase(pObject);

	if (*info.other == "Mouse")
	{
		_currInfo.row = -1;

		if (*info.itSelf == "Combat")
		{
			SetActive(false);

			for (auto row : _armyLayer[Player])
			{
				for (auto Player : row)
				{
					if (nullptr != Player)
						static_cast<Soldier*>(Player)->ChangeAnimation(Soldier::Idle);
				}
			}
		}
	}
}

bool Combat::Initialize()
{
	SetName("Combat");
	SetActive(false);

	// Transform
	_pTransform->SetPosition(Vector3(640.f, 147.5f, 0.f));

	// Texture
	_vecTextures.push_back(Engine::FindTexture(L"UI_BackGround"));
	_pFrontGround = FrontGround::Create(L"UI_FrontGround", Vector3(640.f, 133.f, 0.f));
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pFrontGround);

	// Component
	_pBitmapRenderer = AddComponent<Engine::BitmapRenderer>("BitmapRenderer");
	_pBitmapRenderer->Initialize(Engine::GetBackHDC(), _pTransform);

	int size = _vecTextures[0]->GetImageCount();
	for (int i = 0; i < size; i++)
		_pBitmapRenderer->AddBitmap((*_vecTextures[0])[i], Engine::GetOriginHDC());

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(Timer_End);

	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	// Collider
	Engine::Collider* pCollider = nullptr;

	pCollider = AddCollider("Combat");
	pCollider->SetScale(Vector3(1280.f, 295.f, 0.f));

	pCollider = AddCollider("PlayerRow1");
	pCollider->SetScale(Vector3(200.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(-540.f, 40.f, 0.f));

	pCollider = AddCollider("PlayerRow2");
	pCollider->SetScale(Vector3(200.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(-340.f, 40.f, 0.f));

	pCollider = AddCollider("PlayerRow3");
	pCollider->SetScale(Vector3(200.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(-140.f, 40.f, 0.f));

	pCollider = AddCollider("EnemyRow1");
	pCollider->SetScale(Vector3(850.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(-185.f, 40.f, 0.f));

	pCollider = AddCollider("EnemyRow2");
	pCollider->SetScale(Vector3(200.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(340.f, 40.f, 0.f));

	pCollider = AddCollider("EnemyRow3");
	pCollider->SetScale(Vector3(200.f, 200.f, 0.f));
	pCollider->SetOffset(Vector3(540.f, 40.f, 0.f));

	pCollider = AddCollider("RePlay");
	pCollider->SetScale(Vector3(300.f, 78.f, 0.f));
	pCollider->SetOffset(Vector3(0.f, 392.f, 0.f));
	pCollider->SetActive(false);

	_prevStage = -1;
	_currInfo.row = -1;
	_pMouse = dynamic_cast<Mouse*>(Engine::MyGetObject((int)LayerGroup::UI, "Mouse", "Mouse"));
	_pPlayerStatus = PlayerStatus::Create();

	_pNextButton = NextButton::Create();
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pNextButton);

	_pEnemyAI = EnemyAI::Create();
	Engine::AddObjectInLayer((int)LayerGroup::UI, "EnemyAI", _pEnemyAI);

	_pContinueButton = ContinueButton::Create();
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pContinueButton);

	_pSacrificeButton = SacrificeButton::Create();
	Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", _pSacrificeButton);

	_probabilityAllShotsFired = 20;
	_precisions[0] = 80;
	_precisions[1] = 50;
	_precisions[2] = 60;

	LoadEnemySpawnData();
	LoadRewardSpawnData();
	SpawnEnemy();
	StartSetting();

	Engine::AddObjectInLayer((int)LayerGroup::UI, "Text", FireStepText::Create(Vector3(930.f, 500.f, 0.f)));

	_pTimer->SetActive(BGM, true);

	_pBubble = dynamic_cast<Bubble*>(Engine::MyGetObject((int)LayerGroup::UI, "Section", "Bubble"));
	_pBitFlag->OnFlag(FlagGameClearPopup);

	return true;
}

void Combat::VolleyFire()
{
	if (0 < _rowShotCount)
	{
		if (3 == _rowShotCount)
			_pBubble->BubbleRangedSpeak(Bubble::Alies, 1, 9);

		if (_pBitFlag->CheckFlag(FlagRowShot))
		{
			if (_pTimer->IsOverTime(ShotDelay, 0.5f))
			{
				_pBitFlag->OnFlag(FlagSoldierShot);
				_pBitFlag->OffFlag(FlagRowShot);

				_soldierShotCount = CountAliveSoldiers(Player, _rowShotCount - 1);
				for (auto& Player : _armyLayer[Player][_rowShotCount - 1])
				{
					if (nullptr != Player)
						static_cast<Soldier*>(Player)->ChangeAnimation(Soldier::Fire);
				}

				// 급소 계산
				int rate = rand() % 100;

				if (rate < _probabilityAllShotsFired || _pBitFlag->CheckFlag(FlagBisucuit))
					_pBitFlag->OnFlag(FlagFocusedFired);
			}
		}

		if (_pBitFlag->CheckFlag(FlagSoldierShot))
		{
			if (0 < _soldierShotCount)
			{
				if (_pTimer->IsOverTime(ShotDelay, 0.1f))
				{
					Engine::CameraShake(0.2f, 5.f);
					// 집탄 계산
					int targetRow = _oldInfo.row;

					if (!_pBitFlag->CheckFlag(FlagFocusedFired))
					{
						if (!IsFocusFire(_precisions[3 - _rowShotCount]))
						{
							targetRow = _oldInfo.row - 1;

							if (0 > targetRow)
								targetRow = 1;
						}
					}

					Soldier* pChargeSoldier = FindChargeSoldier();
					if (nullptr != pChargeSoldier)
					{
						CreateMusketHitEffect(pChargeSoldier->GetTransform()->GetPosition());
						pChargeSoldier->SetStateDeath();
						_pEnemyAI->DeleteEnemy(targetRow, pChargeSoldier);
						_alliveEnemy--;
						_soldierShotCount--;
						_pTimer->SetElapsed(ShotDelay, 0.f);

						return;
					}

					int alliveEnemy = CountAliveSoldiers(Enemy, targetRow);

					if (0 == alliveEnemy)
					{
						char name[32] = "";
						sprintf_s(name, "EnemyRow%d", targetRow + 1);
						Engine::Collider* pCollider = FindCollider(name);

						if (nullptr == pCollider)
							return;

						Vector3 position = pCollider->GetPosition();

						if (!strcmp("EnemyRow1", name))
							position.x += 330.f;

						CreateMusketHitEffect(position);
						_soldierShotCount--;
						_pTimer->SetElapsed(ShotDelay, 0.f);

						return;
					}

					int randomIndex = rand() % alliveEnemy;

					if (IsBulletHit(PlayerAccuracy, targetRow, 3 - _rowShotCount) ||
						_pBitFlag->CheckFlag(FlagSacrifice))
					{
						if (!_armyLayer[Enemy][targetRow].empty())
						{
							Army* pObject = _armyLayer[Enemy][targetRow][randomIndex];

							if (pObject->IsStateDeath())
								pObject = FindAlliveEnemy(Enemy, targetRow);

							if (nullptr != pObject)
							{
								CreateMusketHitEffect(pObject->GetTransform()->GetPosition());
								pObject->SetStateDeath();
								_pEnemyAI->DeleteEnemy(targetRow, pObject);

								if (pObject->IsStateDeath())
									_alliveEnemy--;
							}
						}
					}

					_soldierShotCount--;
					_pTimer->SetElapsed(ShotDelay, 0.f);
				}
			}
			else
			{
				_rowShotCount--;
				_pBitFlag->OnFlag(FlagRowShot);
				_pBitFlag->OffFlag(FlagSoldierShot | FlagFocusedFired);
			}
		}
	}
	else
	{
		_pTimer->SetActive(ShotDelay, false);

		if (_pBitFlag->CheckFlag(FlagRowShot))
		{
			_pFlintlock->OnFire();
			_pBitFlag->OffFlag(FlagRowShot | FlagFire | FlagSacrifice | FlagBisucuit);
		}
	}
}

void Combat::LoadEnemySpawnData()
{
	std::fstream fs;
	std::string str;

	fs.open("../Resources/Data/EnemySpawn.csv");
	getline(fs, str);

	int currStage = 0, prevStage = 0;
	int currField = 0, prevField = 0;

	while (true)
	{
		getline(fs, str, ',');

		if ("" == str)
			break;

		currStage = std::stoi(str);
		if (currStage != prevStage)
		{
			prevStage = currStage;
			prevField = currField = 1;
			_enemySpawnInfo.push_back(std::vector<EnemySpawnData>());
			_enemySpawnInfo[currStage - 1].push_back(EnemySpawnData());
		}

		getline(fs, str, ',');
		currField = std::stoi(str);

		if (currField != prevField)
		{
			prevField = currField;
			_enemySpawnInfo[currStage - 1].push_back(EnemySpawnData());
		}

		EnemySpawnInfo spawnInfo;

		getline(fs, str, ',');
		spawnInfo.line = std::stoi(str);

		getline(fs, str, ',');
		spawnInfo.position = std::stof(str);

		getline(fs, str, ',');
		spawnInfo.type = std::stoi(str);

		getline(fs, str, ',');
		spawnInfo.delay = std::stof(str);

		getline(fs, str);
		spawnInfo.startDelay = std::stof(str);

		_enemySpawnInfo[currStage - 1][currField - 1].push_back(spawnInfo);
	}

	fs.close();
}

void Combat::LoadRewardSpawnData()
{
	std::fstream fs;
	std::string str;

	fs.open("../Resources/Data/RewardSpawn.csv");
	getline(fs, str);

	int currStage = 0, prevStage = 0;
	int currField = 0, prevField = 0;

	while (true)
	{
		getline(fs, str, ',');

		if ("" == str)
			break;

		currStage = std::stoi(str);
		if (currStage != prevStage)
		{
			prevStage = currStage;
			prevField = currField = 1;
			_rewardSpawnInfo.push_back(std::vector<RewardSpawnData>());
			_rewardSpawnInfo[currStage - 1].push_back(RewardSpawnData());
		}

		getline(fs, str, ',');
		currField = std::stoi(str);

		if (currField != prevField)
		{
			prevField = currField;
			_rewardSpawnInfo[currStage - 1].push_back(RewardSpawnData());
		}

		RewardSpawnInfo spawnInfo;

		getline(fs, str, ',');
		spawnInfo.position = std::stof(str);

		getline(fs, str, ',');
		spawnInfo.type = static_cast<ItemType>(std::stoi(str));

		getline(fs, str);
		spawnInfo.ID = std::stoi(str);

		_rewardSpawnInfo[currStage - 1][currField - 1].push_back(spawnInfo);
	}

	fs.close();
}

void Combat::DebugEnemyCreate()
{
	if (Engine::IsKeyDown(DIK_PGUP))
	{
		_field++;
		if (_field == _enemySpawnInfo[_stage].size())
		{
			_field = 0;
			_stage++;
		}

		if (_stage == _enemySpawnInfo.size())
		{
			_field = 0;
			_stage = 0;
		}

		SpawnEnemy();
		_pTimer->SetActive(BGM, true);
		_pTimer->SetActive(SFX, true);
		Engine::StopSound((int)SoundGroup::BGM);
		PlayBGM();
	}

	if (Engine::IsKeyDown(DIK_PGDN))
	{
		_field--;
		if (0 > _field)
		{
			_stage--;

			if (0 > _stage)
				_stage = static_cast<int>(_enemySpawnInfo.size() - 1);

			_field = static_cast<int>(_enemySpawnInfo[_stage].size() - 1);
		}

		SpawnEnemy();
		_pTimer->SetActive(BGM, true);
		_pTimer->SetActive(SFX, true);
		Engine::StopSound((int)SoundGroup::BGM);
		PlayBGM();
	}
}

void Combat::SpawnEnemy()
{
	for (auto& row : _armyLayer[Enemy])
	{
		for (auto& Soldier : row)
		{
			if (Soldier)
			{
				Soldier->SetDead();
				Soldier = nullptr;
			}
		}

		row.clear();
		row.shrink_to_fit();

		_alliveEnemy = 0;
	}

	_pEnemyAI->DeleteAllEnemy();

	for (auto& Data : _enemySpawnInfo[_stage][_field])
	{
		if (-1 == Data.line) continue;

		switch (static_cast<EnemyType>(Data.type))
		{
		case EnemyType::Infantry:
		{
			Soldier* pObject = Soldier::Create(Vector3(Data.position, 200.f, 0.f), Soldier::Enemy);
			Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pObject);
			_armyLayer[Enemy][Data.line - 1].push_back(pObject);
			_pEnemyAI->AddEnemy(Data.line - 1, pObject, Data.delay, Data.startDelay);
		}
		break;
		case EnemyType::Canon:
		{
			Canon* pObject = Canon::Create(Vector3(Data.position, 220.f, 0.f));
			Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pObject);
			_armyLayer[Enemy][Data.line - 1].push_back(pObject);
			_pEnemyAI->AddEnemy(Data.line - 1, pObject, Data.delay, Data.startDelay);
		}
		break;
		case EnemyType::Scarecorw:
		{
			Scarecrow* pObject = Scarecrow::Create(Vector3(Data.position, 180.f, 0.f));
			Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pObject);
			_armyLayer[Enemy][Data.line - 1].push_back(pObject);
		}
		break;
		case EnemyType::ChargeInfantry:
			Soldier* pObject = Soldier::Create(Vector3(Data.position, 200.f, 0.f), Soldier::Enemy, true);
			Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pObject);
			_armyLayer[Enemy][Data.line - 1].push_back(pObject);
			_pEnemyAI->AddEnemy(Data.line - 1, pObject, Data.delay, Data.startDelay);
			break;
		}
		_alliveEnemy++;
	}

	_pEnemyAI->SetEnemyStartCount();
}

bool Combat::IsBulletHit(int accuracy, int targetRow, int myRow)
{
	int distance = myRow + targetRow + 2;
	int accuracyRate = 100 - distance * accuracy;
	int rate = rand() % 100;

	if (rate > accuracyRate)
		return false;

	return true;
}

void Combat::InputEvent()
{
	if (Engine::IsKeyPress(Input::DIM_RB))
	{
		if (!_pBitFlag->CheckFlag(FlagFire))
		{
			for (auto row : _armyLayer[Player])
			{
				for (auto Player : row)
				{
					if (nullptr != Player)
						static_cast<Soldier*>(Player)->ChangeAnimation(Soldier::AimStart);
				}
			}
		}

		if (-1 != _currInfo.row && 0 == _rowShotCount)
		{
			if (Engine::IsKeyDown(Input::DIM_LB))
			{						
				if (_pFlintlock->CheckAim() || _isInfinityFire)
				{
					_pTimer->SetActive(ShotDelay, true);
					_currInfo.position = _pMouse->GetMousePosition();
					_oldInfo = _currInfo;

					_rowShotCount = 3;
					_pBitFlag->OnFlag(FlagRowShot | FlagFire);

					if (_pPlayerStatus->_isSacrifice)
					{
						_pPlayerStatus->_isSacrifice = false;
						_pBitFlag->OnFlag(FlagSacrifice);
					}

					if (_pPlayerStatus->_isUseBiscuit)
					{
						_pPlayerStatus->_isUseBiscuit = false;
						_pBitFlag->OnFlag(FlagBisucuit);
					}

					CockPin* pCockPin = static_cast<CockPin*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "CockPin"));
					if (nullptr != pCockPin)
						pCockPin->SetDurablity();
				}
				else
				{
					Engine::MyPlaySound("SFX_SFX_ShotFail", (int)SoundGroup::SFX);
				}
			}
		}
	}
	else if (Engine::IsKeyUp(Input::DIM_RB))
	{
		if (!_pBitFlag->CheckFlag(FlagFire))
		{
			for (auto row : _armyLayer[Player])
			{
				for (auto Player : row)
				{
					if (nullptr != Player)
						static_cast<Soldier*>(Player)->ChangeAnimation(Soldier::Idle);
				}
			}
		}
	}
}

bool Combat::IsFocusFire(int precisions)
{
	int rate = rand() % 100;

	if (rate > precisions)
		return false;

	return true;
}

void Combat::NextField()
{
	if (_pNextButton->IsActive())
	{
		Engine::SetFadeVolume((int)SoundGroup::SFX, 0.f);
		_pTimer->SetActive(SceneChange, true);
		_pNextButton->SetActive(false);

		for (auto& row : _armyLayer[Player])
		{
			for (auto& Player : row)
			{
				if (nullptr != Player)
					static_cast<Soldier*>(Player)->ChangeAnimation(Soldier::Walk);
			}
		}

		for (auto& Soldier : _rewardSoldier)
			Soldier->ChangeAnimation(Soldier::Walk);

		Fade::FadeInfo info;
		info.life = 5.f;
		info.option = Fade::Fade_OutIn;
		info.speed = 2.f;
		info.delay = 1.5f;
		Engine::AddObjectInLayer((int)LayerGroup::UI, "Fade", Fade::Create(info));
	}

	if (_pTimer->IsOverTime(SceneChange, 2.f))
	{
		_field++;

		if (_field == _enemySpawnInfo[_stage].size())
		{
			_field = 0;
			_stage++;
			_pFrontGround->SetIndex(_stage);
		}

		SpawnEnemy();
		_pTimer->SetActive(SceneChange, false);
		_pTimer->SetActive(StageClear, false);
		_pNextButton->SetBattleState(false);
		_pBitFlag->OffFlag(FlagGetReward | FlagStageClear);

		for (auto& Item : _dropItems)
			Item->SetDead();

		for (int i = 2; i >= 0; i--)
		{
			for (int j = 2; j >= 0; j--)
			{
				if (nullptr == _armyLayer[Player][i][j])
				{
					if (!_rewardSoldier.empty())
					{
						_rewardSoldier.front()->SetStartPosition();
						_armyLayer[Player][i][j] = _rewardSoldier.front();
						_rewardSoldier.pop_front();
					}

					continue;
				}

				if (_armyLayer[Player][i][j]->IsStateDeath())
				{
					_armyLayer[Player][i][j]->SetDead();
					_armyLayer[Player][i][j] = nullptr;

					if (!_rewardSoldier.empty())
					{
						_rewardSoldier.front()->SetStartPosition();
						_armyLayer[Player][i][j] = _rewardSoldier.front();
						_rewardSoldier.pop_front();
					}
				}
			}
		}

		for (auto& Soldier : _rewardSoldier)
			Soldier->SetDead();

		_rewardSoldier.clear();
		_dropItems.clear();
		_pTimer->SetActive(BGM, true);
		_pTimer->SetActive(SFX, true);
		_pTimer->SetElapsed(BGM, -0.5f);
		Engine::StopSound((int)SoundGroup::SFX);
	}
}

int Combat::CountAliveSoldiers(int layer, int row)
{
	int soldiers = 0;

	for (auto Object : _armyLayer[layer][row])
	{
		if (nullptr == Object)
			continue;

		if (!Object->IsStateDeath())
			soldiers++;
	}

	return soldiers;
}

void Combat::SpawnReward()
{
	if (_pBitFlag->CheckFlag(FlagGameEnd))
		return;

	if (!_pBitFlag->CheckFlag(FlagGetReward))
	{
		for (auto Item : _rewardSpawnInfo[_stage][_field])
		{
			Vector3 position = { Item.position, 200.f, 0.f };
			Engine::GameObject* pObject = nullptr;

			switch (static_cast<ItemType>(Item.type))
			{
			case ItemType::Rammer:
				break;
			case ItemType::Flint:
				pObject = Flint::Create(position);
				break;
			case ItemType::GunPowder:
				pObject = GunPowder::Create(position);
				break;
			case ItemType::Ammunition:
				pObject = Ammunition::Create(position);
				break;
			case ItemType::PaperCartridge:
				pObject = PaperCartidge::Create(position);
				break;
			case ItemType::Biscuit:
				pObject = Biscuit::Create(position, _pPlayerStatus);
				break;
			case ItemType::Letter:
				pObject = Letter::Create(position, Item.ID);
				break;
			case ItemType::Infantry:
				SpawnRewardInfantry(Item.ID);
				break;
			}

			Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", pObject);
		}

		_pBitFlag->OnFlag(FlagGetReward);
	}
}

void Combat::StartSetting()
{
	Soldier* pSoldier = nullptr;

	for (auto& row : _armyLayer[Player])
	{
		row.resize(3);
	}

	for (int i = 0; i < 9; i++)
	{
		if (2 > i) continue;

		float offset = 200.f * (i / 3);
		pSoldier = Soldier::Create(Vector3(60.f + AllignOffset * (i % 3) + offset, 200.f, 0.f), Soldier::Player);
		Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pSoldier);
		_armyLayer[Player][int(i / 3)][i % 3] = pSoldier;
	}

	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Flintlock::Create(Vector3(600.f, 540.f, 0.f)));
	_pFlintlock = dynamic_cast<Flintlock*>(Engine::MyGetObject((int)LayerGroup::Object, "Musket", "Flintlock"));
	StartInventory();
}

void Combat::StartInventory()
{
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 400.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 400.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 400.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(50.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(150.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Ammunition::Create(Vector3(250.f, 450.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", PaperCartidge::Create(Vector3(80.f, 520.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Flint::Create(Vector3(160.f, 520.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Biscuit::Create(Vector3(230.f, 520.f, 0.f), _pPlayerStatus));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", GunPowder::Create(Vector3(170.f, 600.f, 0.f)));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Letter::Create(Vector3(230.f, 680.f, 0.f), 1));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Letter::Create(Vector3(180.f, 680.f, 0.f), 2));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Letter::Create(Vector3(120.f, 680.f, 0.f), 5));
	Engine::AddObjectInLayer((int)LayerGroup::Object, "Musket", Letter::Create(Vector3(60.f, 680.f, 0.f), 6));
}

Army* Combat::FindAlliveEnemy(int layer, int row)
{
	for (auto& Object : _armyLayer[layer][row])
	{
		if (nullptr == Object)
			continue;

		if (!Object->IsStateDeath())
			return Object;
	}

	return nullptr;
}

void Combat::SpawnRewardInfantry(int count)
{
	for (int i = 8; i >= 0; i--)
	{
		Army* pArmy = _armyLayer[Player][int(i / 3)][i % 3];

		if (nullptr == pArmy || pArmy->IsStateDeath())
		{
			float offset = 200.f * (i / 3);
			Soldier* pSoldier = Soldier::Create(Vector3(60.f + AllignOffset * (i % 3) + offset, 200.f, 0.f), Soldier::SpawnSoldier);
			_rewardSoldier.push_back(pSoldier);
			Engine::AddObjectInLayer((int)LayerGroup::Object, "Soldier", pSoldier);
			count--;
		}

		if (0 == count)
			return;
	}
}

void Combat::EnemyVolleyFire()
{
	int enemyRow = _pEnemyAI->GetAttackRow();
	if (-1 != enemyRow)
	{
		_enemyShoutCount = _pEnemyAI->GetAttackCount();
		_enemyAttackRow = enemyRow;
		_pTimer->SetActive(EnemyShotDelay, true);
	}

	if (0 < _enemyShoutCount)
	{
		if (_pTimer->IsOverTime(EnemyShotDelay, 0.1f))
		{
			_enemyShoutCount--;
			_pTimer->SetElapsed(EnemyShotDelay, 0.f);
			Engine::CameraShake(0.2f, 5.f);

			int targetRow = 2 - rand() % 3;
			int alliveSoldier = CountAliveSoldiers(Player, targetRow);

			if (0 == alliveSoldier)
			{
				char name[32] = "";
				sprintf_s(name, "PlayerRow%d", targetRow + 1);
				Engine::Collider* pCollider = FindCollider(name);

				if (nullptr == pCollider)
					return;

				CreateMusketHitEffect(pCollider->GetPosition());

				return;
			}

			int randomIndex = rand() % alliveSoldier;

			if (IsBulletHit(EnemyAccuracy, targetRow, _enemyAttackRow))
			{
				if (!_armyLayer[Player][targetRow].empty())
				{
					Army* pSoldier = _armyLayer[Player][targetRow][randomIndex];

					if (nullptr == pSoldier || pSoldier->IsStateDeath())
						pSoldier = FindAlliveEnemy(Player, targetRow);

					if (nullptr != pSoldier)
					{
						CreateMusketHitEffect(pSoldier->GetTransform()->GetPosition());
						pSoldier->SetStateDeath();
					}
				}
			}
		}
	}

	if (_pEnemyAI->IsAttackCanon())
	{
		int targetRow = 2 - rand() % 3;
		int alliveSoldier = CountAliveSoldiers(Player, targetRow);

		char name[32] = "";
		sprintf_s(name, "PlayerRow%d", targetRow + 1);
		Engine::Collider* pCollider = FindCollider(name);

		if (nullptr == pCollider)
			return;

		_pTimer->SetActive(CanonDelay, true);
		_busterPosition = pCollider->GetPosition();

		if (!_armyLayer[Player][targetRow].empty())
		{
			for (auto& Soldier : _armyLayer[Player][targetRow])
			{
				if (nullptr == Soldier)
					continue;

				Soldier->SetStateDeath();
			}
		}
	}

	if (_pTimer->IsOverTime(CanonDelay, 0.5f))
	{
		Effect::EffectInfo info;
		info.renderGroup = RenderGroup::Effect;
		info.position = _busterPosition;
		info.textureTag = L"Effect_HitFire_canon";
		info.aniSpeed = 1.5f;
		Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
		_pTimer->SetActive(CanonDelay, false);
	}
}

void Combat::Sacrifice()
{
	if (0 >= _allivePlayer || _pNextButton->GetBattleState())
	{
		_pPlayerStatus->_isSacrifice = false;
		_pSacrificeButton->SetActive(false);
		return;
	}

	if (_pSacrificeButton->IsActive())
	{
		_pBubble->BubbleRangedSpeak(Bubble::Alies, 21, 24);

		for (int i = 2; i >= 0; i--)
		{
			for (int j = 2; j >= 0; j--)
			{
				if (nullptr == _armyLayer[Player][i][j] ||
					_armyLayer[Player][i][j]->IsStateDeath())
					continue;

				_armyLayer[Player][i][j]->SetStateDeath();
				_pPlayerStatus->_isSacrifice = true;
				_pSacrificeButton->SetActive(false);
				CreateMusketHitEffect(_armyLayer[Player][i][j]->GetTransform()->GetPosition());
				return;
			}
		}
	}
}

void Combat::GameOver()
{
	_allivePlayer = 0;

	for (int i = 0; i < Row; i++)
		_allivePlayer += CountAliveSoldiers(Player, i);

	if (0 >= _allivePlayer)
	{
		if (!_isLose)
		{
			_pBubble->BubbleRangedSpeak(Bubble::Alies, 15, 20);
			Engine::MyPlaySound("BGM_BGM_GameOver", (int)SoundGroup::SFX);
			_isLose = true;
		}

		_pContinueButton->SetActive(true);
		_pBitFlag->OffFlag(FlagStageClear);

		if (_pContinueButton->_isRestart)
		{
			Engine::ClearLayer((int)LayerGroup::Object);

			for (auto& row : _armyLayer[Player])
			{
				row.clear();
				row.shrink_to_fit();
			}

			_pEnemyAI->DeleteAllEnemy();
			_field = 0;
			StartSetting();
			SpawnEnemy();
			static_cast<FireStepText*>(Engine::MyGetObject((int)LayerGroup::UI, "Text", "FireSteopText"))->SetTarget();

			_pContinueButton->_isRestart = false;
			_pContinueButton->SetActive(false);
			_isLose = false;
		}
	}
}
void Combat::GameClear()
{
	if (0 >= _allivePlayer)
	{
		_pNextButton->SetBattleState(false);
		return;
	}

	if (_pBitFlag->CheckFlag(FlagStageClear))
	{
		if (_stage < _enemySpawnInfo.size())
		{
			_pNextButton->SetBattleState(true);
			SpawnReward();
			NextField();
		}
	}
	else
	{
		if (0 >= _alliveEnemy)
		{
			Engine::SetFadeVolume((int)SoundGroup::BGM, 0.4f);
			_pTimer->SetActive(StageClear, true);

			if (2 == _stage && 1 == _field)
			{
				_pBitFlag->OnFlag(FlagGameEnd);
				_pTimer->SetActive(GameEnd, true);

				if (_pBitFlag->CheckFlag(FlagGameClearPopup))
				{
					if (_pTimer->IsOverTime(GameEnd, 3.f))
					{
						_pBitFlag->OffFlag(FlagGameClearPopup);
						Engine::MyPlaySound("BGM_BGM_Clear", (int)SoundGroup::SFX);
						Engine::AddObjectInLayer((int)LayerGroup::UI, "UI", Letterhead::Create(Vector3(640.f, 360.f, -10.f), 4, nullptr));
						Engine::Collider* pCollider = FindCollider("RePlay");
						pCollider->SetActive(true);
					}
				}
			}
			else
			{
				if (_pTimer->IsOverTime(StageClear, 3.f))
				{
					_pBitFlag->OnFlag(FlagStageClear);
					_pBubble->BubbleRangedSpeak(Bubble::Alies, 10, 14);
					Engine::StopSound((int)SoundGroup::BGM);
					Engine::MyPlaySound("BGM_BGM_Clear", (int)SoundGroup::SFX);
				}
			}
		}
	}
}

void Combat::CreateMusketHitEffect(const Vector3& position)
{
	Effect::EffectInfo info;
	info.renderGroup = RenderGroup::Effect;
	info.position = position;
	info.position.x += float(20 - rand() % 40);
	info.position.y += float(20 - rand() % 40);
	info.textureTag = L"Effect_HitFire_musket";
	info.aniSpeed = 0.1f;
	Engine::AddObjectInLayer((int)LayerGroup::Effect, "Effect", Effect::Create(info));
}

void Combat::PlayBGM()
{
	if (_pTimer->IsOverTime(BGM, 1.f))
	{
		Engine::SetVolume((int)SoundGroup::BGM, 1.f);
		switch (_stage)
		{
		case 0:
			Engine::MyPlaySound("BGM_BGM_Tutorial", (int)SoundGroup::BGM, true);
			break;
		case 1:
			Engine::MyPlaySound("BGM_BGM_Ingame1", (int)SoundGroup::BGM, true);
			break;
		case 2:
			if (1 == _field)
			{
				Engine::MyPlaySound("BGM_BGM_Ingame2", (int)SoundGroup::BGM, true);
			}
			else
			{
				Engine::MyPlaySound("BGM_BGM_Ingame1", (int)SoundGroup::BGM, true);
			}
			break;
		}

		_pTimer->SetActive(BGM, false);
	}

	if (_pTimer->IsOverTime(SFX, 0.4f))
	{
		Engine::SetVolume((int)SoundGroup::SFX, 1.f);
		Engine::MyPlaySound("SFX_SFX_WarStart", (int)SoundGroup::SFX);
		_pTimer->SetActive(SFX, false);

		switch (_stage)
		{
		case 0:
			if (1 == _field)
			{
				Engine::AddObjectInLayer((int)LayerGroup::UI, "Section",
					Letterhead::Create(Vector3(620.f, 330.f, -110.f), 2, nullptr));

				Engine::AddObjectInLayer((int)LayerGroup::UI, "Section",
					Letterhead::Create(Vector3(660.f, 390.f, -100.f), 5, nullptr));
			}
			break;
		}
	}
}

void Combat::Free()
{
	Engine::SafeRelease(_pPlayerStatus);
}

Soldier* Combat::FindChargeSoldier()
{
	for (auto& row : _armyLayer[Enemy])
	{
		for (auto& army : row)
		{
			Soldier* pSoldier = dynamic_cast<Soldier*>(army);

			if (nullptr != pSoldier)
			{
				if (pSoldier->IsStateDeath())
					continue;

				if (pSoldier->IsChargetAttack())
					return pSoldier;
			}
		}
	}

	return nullptr;
}

Combat* Combat::Create()
{
	Combat* pInstance = new Combat;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return nullptr;
}
