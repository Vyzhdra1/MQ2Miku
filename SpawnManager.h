#pragma once
#include <mq/Plugin.h>
#include "PlayerUtils.h"
#include "HealingManager.h"

class SpawnManager
{
private:
	inline static SpawnManager* _Manager = 0;
	int _AttackTargetId = -1;
public:
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
		case ATTACK_TARGET: return _AttackTargetId == -1 ? 0 : GetSpawnByID(_AttackTargetId);
		}

		return 0;
	}

	void SetAttackTarget(int aAttackId) {
		_AttackTargetId = aAttackId;
	}

	int GetRealGroupMemberCount() {
		if (!pLocalPC->Group) {
			return 0;
		}
		return pLocalPC->Group->GetNumberOfMembers();
	}

	int GetAverageGroupHealth() {
		return HealingManager::Get()->GetAverageGroupHealth();
	}

	void SpawnManager::Assess() {
		HealingManager::Get()->Assess();
	}

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