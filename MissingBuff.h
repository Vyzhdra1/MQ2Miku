#pragma once
#include "Ability.h"
#include <mq/Plugin.h>
#include "Condition.h"
#include "BooleanCondition.h"
#include "Utils.h"
class MissingBuff 
{
protected:
	SpawnType _SpawnType = UNKNOWN;
	PlayerClient* _Spawn = 0;
	int _BuffID = 0;
	bool _HasBuff = true;
public:
	MissingBuff(int aBuffID, SpawnType aSpawnType) {
		_BuffID = aBuffID;
		_SpawnType = aSpawnType;
	}

	void Validate() {
		PlayerClient * lSpawn = GameManager::GetAssessmentManager()->GetGroupAssessment()->GetSpawn(_SpawnType);
		
		_HasBuff = true;

		if (!lSpawn) return;
	}

	bool ConditionMet(Ability* aAbility) {
		PlayerClient* lSpawn = GameManager::GetAssessmentManager()->GetGroupAssessment()->GetSpawn(_SpawnType);

		if (!lSpawn) return false;

		return UnderlyingConditionMet(aAbility, lSpawn);
	}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) = 0;
};
