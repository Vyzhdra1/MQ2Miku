#pragma once
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class CastingCondition : public BooleanCondition
{
public:
	static const char* Key;
public:
	CastingCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return Utils::IsCastWindowShowing() == _BooleanCondition;
	}
};
const char* CastingCondition::Key = "casting";

