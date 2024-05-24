#pragma once
#ifndef H_InvisibleCondition
#define H_InvisibleCondition
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"
#include <set>

class InvisibleCondition : public BooleanCondition
{
public:
	static const char* Key;

	InvisibleCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return PlayerUtils::Invisible() == _BooleanCondition;
	}
};
const char* InvisibleCondition::Key = "invis";

#endif