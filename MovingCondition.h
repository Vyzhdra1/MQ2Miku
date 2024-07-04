#pragma once
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"
#include "MikuMovementUtils.h"

class MovingCondition : public BooleanCondition
{
public:
	static const char* Key;
	int _IntegerValue = 0;
public:
	MovingCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return (MikuMovementUtils::IsMoving() == _BooleanCondition) && (MikuMovementUtils::GetTimeSinceLastMove() > _IntegerValue);
	}

	virtual void ParseNextValue(std::string aValue) override {
		_IntegerValue = Utils::StrToInt(aValue.c_str(), 0);

		if (_IntegerValue == -1) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
		}
	}
};
const char* MovingCondition::Key = "moving";
