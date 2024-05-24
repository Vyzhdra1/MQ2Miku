#pragma once
#include "PlayerUtils.h"
#include "SpawnCondition.h"

class DeadPlayerCondition : public SpawnCondition
{
public:
	static const char* Key;

	DeadPlayerCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {}

	bool ConditionMet(Ability* aAbility) {
		return (SpawnManager::Get()->GetSpawn(DEAD_PLAYER) != 0) == _BooleanCondition;
	}
	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) { return false; }
};

const char* DeadPlayerCondition::Key = "deadplayer";
