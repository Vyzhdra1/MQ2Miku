#pragma once
#include "BooleanCondition.h"
#include "SpellUtils.h
#include "Ability.h"

class AuraCondition : public BooleanCondition {
public:
	inline static const std::string Key = "aura";

	AuraCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return SpellUtils::HasAura(aAbility->GetSpell()->Name) == _BooleanCondition;
	}
};