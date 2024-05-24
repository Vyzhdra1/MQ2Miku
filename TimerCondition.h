#pragma once
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class TimerCondition : public BooleanCondition
{
public:
	static const char* Key;

	TimerCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return PlayerUtils::IsTimerState() == _BooleanCondition;
	}
};

const char* TimerCondition::Key = "timer";
