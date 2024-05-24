#pragma once
#include "Ability.h"
#include "AbilityManager.h"
#include "BooleanCondition.h"
#include "Utils.h"

class AbilityCondition : public BooleanCondition {
protected:
	Ability* _Ability = 0;
public:
	AbilityCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	virtual void ParseNextValue(std::string aValue) override {
		_Ability = AbilityManager::Get()->GetAbility(aValue);

		if (!_Ability) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown Identifier: ", aValue);
			return;
		}
	}
};