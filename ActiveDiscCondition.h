#pragma once
#include "AbilityCondition.h"
#include "Utils.h"

class ActiveDiscCondition : public AbilityCondition {
public:
	inline static const std::string Key = "discactive";

	ActiveDiscCondition(std::set<std::string> aSettings) : AbilityCondition(aSettings) {}

	virtual bool ConditionMet(Ability* aAbility) override {
		if (_Ability == 0) return (bool)Utils::ActiveDiscipline() == _BooleanCondition;

		if (!_Ability->GetSpell()) return false;

		return (Utils::ActiveDiscipline() == _Ability->GetSpellID()) == _BooleanCondition;
	}
};
