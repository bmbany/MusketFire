#include "EnemyAI.h"
#include "Soldier.h"
#include "Canon.h"

#include "Bubble.h"
#include "BitFlag.h"
#include "Timer.h"
#include "Client_Define.h"

int EnemyAI::Update(const float& deltaTime)
{
	_attackRow = -1;
	_attackCount = 0;
	_isAttackCanon = false;

	for (int i = 0; i < 3; i++)
	{
		for (auto& AI : _enemys[i])
		{
			AI.elapsed += deltaTime;

			if (AI.isFirst)
			{
				if (AI.elapsed >= AI.startDelay)
				{
					AI.isFirst = false;
					AI.elapsed = 0.f;
				}

				continue;
			}
			//bubble
			if (!_isDefeat &&
				_alliveEnemy == 0 &&
				!_pBubble->IsBubbleRendering(Bubble::Enemy))
			{
				_pBubble->BubbleRangedSpeak(Bubble::Enemy, 12, 16);
				_isDefeat = true;
			}

			if ((*_enemys)[0].elapsed >= (*_enemys)[0].delay - 2.5f &&
				(*_enemys)[0].elapsed < (*_enemys)[0].delay &&
				_alliveEnemy != 0 &&
				!_pBubble->IsBubbleRendering(Bubble::Enemy))
			{
				_pBubble->BubbleRangedSpeak(Bubble::Enemy, 3, 4);
			}

			if ((*_enemys)[0].elapsed >= (*_enemys)[0].delay - 2.f &&
				(*_enemys)[0].elapsed < (*_enemys)[0].delay &&
				_alliveEnemy != 0 &&
				!_pBubble->IsBubbleRendering(Bubble::Enemy))
			{
				_pBubble->BubbleRangedSpeak(Bubble::Enemy, 5, 9);
			}

			if (AI.elapsed >= AI.delay)
			{
				if (nullptr != AI.pSoldier)
				{
					if (AI.pSoldier->_isChargeInfantry)
					{
						if (0.6f >= _alliveEnemy / _startEnemyCount)
						{
							_pBubble->BubbleRangedSpeak(Bubble::Enemy, 10, 11);
							AI.pSoldier->ChargeAttack();
							AI.elapsed = 0.f;
							continue;
						}
					}
					AI.pSoldier->ChangeAnimation(Soldier::AimStart);

					if (AI.elapsed >= AI.delay + 1.f)
					{
						AI.pSoldier->ChangeAnimation(Soldier::Fire);
						AI.elapsed = 0.f;
						_attackRow = i;
						_attackCount++;
					}
				}

				if (nullptr != AI.pCanon)
				{
					if (1 == _enemys[i].size())
					{
						AI.pCanon->SetStateDeath();
						AI.pCanon = nullptr;
						_alliveEnemy--;
						continue;
					}

					AI.pCanon->Attack();
					AI.elapsed = 0.f;
					_attackRow = i;
					_isAttackCanon = true;
				}
			}
		}
	}

	return 0;
}

int EnemyAI::LateUpdate(const float& deltaTime)
{
	return 0;
}

void EnemyAI::Render(Gdiplus::Graphics* pGraphics)
{
}

void EnemyAI::AddRenderer()
{
}

void EnemyAI::AddEnemy(int row, Soldier* pSoldier, float delay, float startDelay)
{
	_enemys[row].push_back(AIInfo(pSoldier, delay, startDelay));
	_isDefeat = false;
	_alliveEnemy++;
}

void EnemyAI::AddEnemy(int row, Canon* pCanon, float delay, float startDelay)
{
	_enemys[row].push_back(AIInfo(pCanon, delay, startDelay));
	_alliveEnemy++;
}

void EnemyAI::DeleteEnemy(int row, Army* pArmy)
{
	for (auto& AI : _enemys[row])
	{
		if (pArmy == AI.pSoldier ||
			pArmy == AI.pCanon)
		{
			_alliveEnemy--;
			AI.pSoldier = nullptr;
			AI.pCanon = nullptr;
			break;
		}
	}
}

void EnemyAI::DeleteAllEnemy()
{
	for (auto& Soldier : _enemys)
	{
		_alliveEnemy = 0;
		Soldier.clear();
		Soldier.shrink_to_fit();
	}
}

bool EnemyAI::Initialize()
{
	// Component
	_pBitFlag = AddComponent<Engine::BitFlag>("BitFlag");

	_pTimer = AddComponent<Engine::Timer>("Timer");
	_pTimer->AddTimer(Timer_End);
	_pTimer->SetActive(FSM, true);

	_pBubble = dynamic_cast<Bubble*>(Engine::MyGetObject((int)LayerGroup::UI, "Section", "Bubble"));

	return true;
}

void EnemyAI::Free()
{
}

EnemyAI* EnemyAI::Create()
{
	EnemyAI* pInstance = new EnemyAI;
	if (pInstance->Initialize())
		return pInstance;

	Engine::SafeRelease(pInstance);
	return pInstance;
}
