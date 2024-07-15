#pragma once
#pragma once
#include "Ability.h"
#include "SpawnCondition.h"
#include "PlayerUtils.h"
#include <set>

class MezzedCondition : public SpawnCondition
{
private:
	std::set<int> _MezAnimations{ 26, 32, 71, 72, 110, 111 };
public:
	static const char* Key; 

	MezzedCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {	}

	bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) override {
		return (_MezAnimations.count(aSpawn->Animation) != 0) == _BooleanCondition;
	}
};
const char* MezzedCondition::Key = "mezzed";
