#pragma once
#include "Section.h"

namespace Engine
{
	class Timer;
	class BitFlag;
	class BitmapRenderer;
}

class Bubble;
class Mouse;
class Army;
class Soldier;
class Flintlock;
class NextButton;
class ContinueButton;
class SacrificeButton;
class EnemyAI;
class PlayerStatus;
class FrontGround;
class Combat : public Section
{
#define Row 3
#define MaxSoldier 5
#define AllignOffset 40.f
#define PlayerAccuracy 5
#define EnemyAccuracy 10

	enum class EnemyType { Infantry, Canon, Scarecorw, ChargeInfantry };
	enum class ItemType { Rammer, Flint, GunPowder, Ammunition, PaperCartridge, Biscuit, Letter, Infantry };
	enum Layer { Player, Enemy, Layer_End };
	enum Timer { ShotDelay, SceneChange, StageClear, EnemyShotDelay, CanonDelay, BGM, SFX, GameEnd, Timer_End };
	enum Flag
	{
		FlagRowShot = 1 << 1,
		FlagSoldierShot = 1 << 2,
		FlagFocusedFired = 1 << 3,
		FlagGetReward = 1 << 4,
		FlagFire = 1 << 5,
		FlagStageClear = 1 << 6,
		FlagBisucuit = 1 << 7,
		FlagSacrifice = 1 << 8,
		FlagGameEnd = 1 << 9,
		FlagGameClearPopup = 1 << 10
	};
	struct ShotInfo
	{
		Vector3 position;
		int row;
	};
	struct EnemySpawnInfo
	{
		int type = 0;
		int line = 0;
		float position = 0;
		float delay = 0.f;
		float startDelay = 0.f;
	};
	struct RewardSpawnInfo
	{
		float position = 0;
		ItemType type;
		int ID = 0;
	};

	using EnemySpawnData = std::vector<EnemySpawnInfo>;
	using RewardSpawnData = std::vector<RewardSpawnInfo>;

private:
	explicit Combat() = default;
	virtual ~Combat() = default;

public:
	// GameObject을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;
	void Render(Gdiplus::Graphics* pGraphics) override;
	void AddRenderer() override;
	void OnCollision(CollisionInfo info) override;
	void OnCollisionExit(CollisionInfo info) override;

private:
	bool Initialize();
	void StartSetting();
	void StartInventory();
	void LoadEnemySpawnData();
	void LoadRewardSpawnData();
	void DebugEnemyCreate();
	void PlayBGM();

	void InputEvent();
	void GameOver();
	void GameClear();
	void Sacrifice();
	void NextField();
	void SpawnReward();
	void SpawnRewardInfantry(int count);
	void SpawnEnemy();
	void VolleyFire();
	void EnemyVolleyFire();
	bool IsBulletHit(int accuracy, int targetRow, int myRow);
	bool IsFocusFire(int precisions);
	Soldier* FindChargeSoldier();
	int CountAliveSoldiers(int layer, int row);
	Army* FindAlliveEnemy(int layer, int row);
	void CreateMusketHitEffect(const Vector3& position);

	void Free() override;
public:
	static Combat* Create();

private:
	std::vector<std::vector<EnemySpawnData>>	_enemySpawnInfo;
	std::vector<std::vector<RewardSpawnData>>	_rewardSpawnInfo;
	std::vector<Army*>							_armyLayer[Layer_End][Row];
	std::set<Engine::GameObject*>				_dropItems;
	std::list<Soldier*>							_rewardSoldier;

	Engine::BitmapRenderer* _pBitmapRenderer = nullptr;
	Engine::BitFlag* _pBitFlag = nullptr;
	Engine::Timer* _pTimer = nullptr;

	EnemyAI* _pEnemyAI = nullptr;
	Flintlock* _pFlintlock = nullptr;
	NextButton* _pNextButton = nullptr;
	ContinueButton* _pContinueButton = nullptr;
	PlayerStatus* _pPlayerStatus = nullptr;
	SacrificeButton* _pSacrificeButton = nullptr;
	FrontGround* _pFrontGround = nullptr;
	Bubble* _pBubble = nullptr;

	ShotInfo	_currInfo;
	ShotInfo	_oldInfo;
	Vector3		_busterPosition;
	int			_enemyAttackRow = 0;
	int			_enemyShoutCount = 0;
	int			_rowShotCount = 0;
	int			_soldierShotCount = 0;
	int			_probabilityAllShotsFired = 0;
	int			_precisions[Row];

	int			_field = 0;
	int			_stage = 0;
	int			_prevStage = 0;
	int			_alliveEnemy = 0;
	int			_allivePlayer = 0;

	// Debug
	bool _isInfinityFire = false;

	// Sound
	bool _isLose = false;
	bool _isRePlay = false;
};

