#pragma once
#ifndef H_HealthCondition
#define H_HealthCondition
#include "PlayerUtils.h"
#include "SpawnCondition.h"
#include "SpawnManager.h"

class HealthCondition : public SpawnCondition
{
public:
	static const char* Key;

	HealthCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {}

	bool ConditionMet(Ability* aAbility) {
		if (_SpawnType == GROUP) {
			int lAverageGroupHealth = SpawnManager::Get()->GetAverageGroupHealth();

			return (lAverageGroupHealth >= _IntegerValue) == _BooleanCondition;
		}

		return SpawnCondition::ConditionMet(aAbility);
	}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) {
		return (PlayerUtils::ValuePerc(aSpawn->HPMax, aSpawn->HPCurrent) >= _IntegerValue) == _BooleanCondition;
	}
};

const char* HealthCondition::Key = "hp>";

#endif