#pragma once
#ifndef H_StandingCondition
#define H_StandingCondition
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class StandingCondition : public BooleanCondition
{
public:
	static const char* Key;
public:
	StandingCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return PlayerUtils::IsStanding() == _BooleanCondition;
	}
};
const char* StandingCondition::Key = "standing";

#endif
