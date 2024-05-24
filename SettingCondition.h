#pragma once
#include "BooleanCondition.h"
#include "GameManager.h"

class SettingCondition : public BooleanCondition
{
private:
	std::string _Value = "";
	std::string _Setting = "";
public:
	static const char* Key;

	SettingCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return SettingManager::Get()->IsValueMatched(_Setting, _Value) == _BooleanCondition;
	}

	virtual void ParseNextValue(std::string aValue) override {
		_Setting = aValue;
	}
};

const char* SettingCondition::Key = "setting";


