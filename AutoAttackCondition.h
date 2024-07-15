#pragma once
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class AutoAttackCondition : public BooleanCondition
{
public:
	static const char* Key;

	AutoAttackCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return  (pEverQuestInfo->bAutoRangeAttack || pEverQuestInfo->bAutoAttack) == _BooleanCondition;
	}
};

const char* AutoAttackCondition::Key = "autoattackon";