#pragma once
#include "AbilityCondition.h"

class AbilityReadyCondition : public AbilityCondition {
public:
	inline static const std::string Key = "abilityready";

	AbilityReadyCondition(std::set<std::string> aSettings) : AbilityCondition(aSettings) {}

	virtual bool ConditionMet(Ability* aAbility) override {
		if (!_Ability) return false;
		if (!_Ability->GetSpell()) return false;

		return _Ability->AbilityReady();
	}
};