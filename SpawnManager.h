#pragma once
#include <mq/Plugin.h>
#include "PlayerUtils.h"
#include "HealingManager.h"

class SpawnManager
{
private:
	inline static SpawnManager* _Manager = 0;
//	int _AverageGroupHealth;
//	PlayerClient* _Tank;
//	int _TankHealth;
//	int _LowestHealthPerc;
//	PlayerClient* _LowestHealthPlayer;
//	int _GroupMemberCount;
//	int _RealGroupMemberCount;
//	PlayerClient * _TankTarget;
//	PlayerClient* _DeadPlayer;
public:
/*	PlayerClient* GetSpawn(SpawnType aSpawnType) {
		if (aSpawnType == GROUP) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Error getting spawn by key: GROUP");
		}

		switch (aSpawnType) {
		case TANK: return _Tank;
		case TARGET_OF_TANK: return _TankTarget;
		case LOWEST_HP_GROUP: return _LowestHealthPlayer;
		case DEAD_PLAYER: return _DeadPlayer;
		case SELF: return GetCharInfo()->pSpawn;
		case MYTARGET: return pTarget;
		}

		return 0;
	}*/

	PlayerClient* GetSpawn(SpawnType aSpawnType) {
		if (aSpawnType == GROUP) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Error getting spawn by key: GROUP");
		}

		switch (aSpawnType) {
		case TANK: return HealingManager::Get()->GetSpawn(aSpawnType);
		case TARGET_OF_TANK: return HealingManager::Get()->GetSpawn(aSpawnType);
		case LOWEST_HP_GROUP: return HealingManager::Get()->GetSpawn(aSpawnType);
		case DEAD_PLAYER: return HealingManager::Get()->GetSpawn(aSpawnType);
		case SELF: return GetCharInfo()->pSpawn;
		case MYTARGET: return pTarget;
		}

		return 0;
	}

	int GetRealGroupMemberCount() {
		if (!pLocalPC->Group) {
			return 0;
		}
		return pLocalPC->Group->GetNumberOfMembers();
	}

	int GetAverageGroupHealth() {
		return HealingManager::Get()->GetAverageGroupHealth();//_AverageGroupHealth;
	}

	void SpawnManager::Assess() {
		HealingManager::Get()->Assess();
	}
	/*
	void SpawnManager::Assess() {
		_RealGroupMemberCount = 0;
		_GroupMemberCount = 0;
		_LowestHealthPerc = 100;
		_AverageGroupHealth = 100;

		_TankHealth = 100;
		_Tank = 0;
		_LowestHealthPlayer = 0;
		_TankTarget = 0;
		_DeadPlayer = 0;

		int lCurrentHealthPerc = 200;
		int lTotalGroupHealth = 0;

		CGroupMember** lIterator;

		if (!pLocalPC->Group) {
			return;
		}

		for (lIterator = pLocalPC->Group->begin(); lIterator != pLocalPC->Group->end(); lIterator++)
		{
			if (!(*lIterator)) {
				return;
			}

			PlayerClient* lGroupMember = (*lIterator)->pSpawn;

			if (!lGroupMember) {
				continue;
			}

			_RealGroupMemberCount++;

			if (DistanceToSpawn(pLocalPC->pSpawn, lGroupMember) > 250) {
				continue;
			}

			if (lGroupMember->StandState == STANDSTATE_DEAD) {
				if (DistanceToSpawn(pLocalPC->pSpawn, lGroupMember) < 100) {
					_DeadPlayer = lGroupMember;
				}
				continue;
			}

			if ((*lIterator)->IsMainTank()) {
				_Tank = lGroupMember;

				_TankHealth = PlayerUtils::ValuePerc(_Tank->HPMax, _Tank->HPCurrent);
				int lTankTargetID = _Tank->TargetOfTarget;
				if (lTankTargetID > 0) {
					_TankTarget = GetSpawnByID(lTankTargetID);
				}
				continue;
			}

			_GroupMemberCount++;

			int lCurrentHealthPerc = PlayerUtils::ValuePerc(lGroupMember->HPMax, lGroupMember->HPCurrent);
			if (lCurrentHealthPerc < _LowestHealthPerc) {
				_LowestHealthPerc = lCurrentHealthPerc;
				_LowestHealthPlayer = lGroupMember;
			}

			lTotalGroupHealth += lCurrentHealthPerc;
		}

		if (_GroupMemberCount <= 2) {
			_AverageGroupHealth = 100;
		}
		else {
			_AverageGroupHealth = lTotalGroupHealth / _GroupMemberCount;
		}
	}*/

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static SpawnManager* Get() {
		if (!_Manager) {
			_Manager = new SpawnManager();
		}
		return _Manager;
	}
};