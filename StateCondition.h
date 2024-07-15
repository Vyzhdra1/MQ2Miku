#pragma once
#ifndef H_StateCondition
#define H_StateCondition
#include "Ability.h"
#include "Condition.h"
#include "StateManager.h"
#include "BooleanCondition.h"
#include "Utils.h"
class StateCondition : public BooleanCondition
{
private:
	std::set<MikuState> _State;
	const std::string COMBAT_STR = "combat";
	const std::string ATTACK_STR = "attack";
	const std::string BURN_STR = "burn";
	const std::string OOC_STR = "ooc";
	const std::string BACK_OFF_STR = "backoff";
	const std::string FOLLOW_STR = "follow";
public:
	static const char* Key;

	StateCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		for (std::set<MikuState>::iterator lIterator = _State.begin(); lIterator != _State.end(); lIterator++) {
			if (StateManager::Get()->IsStateActive(*lIterator)) {
				return _BooleanCondition;
			}
		}

		return !_BooleanCondition;
	}

	virtual void ParseNextValue(std::string aValue) override {
		if (!COMBAT_STR.compare(aValue.c_str())) {
			_State.insert(IN_COMBAT);
		}
		else if (!ATTACK_STR.compare(aValue.c_str())) {
			_State.insert(STATE_ATTACK);
		}
		else if (!BURN_STR.compare(aValue.c_str())) {
			_State.insert(BURN);
		}
		else if (!BACK_OFF_STR.compare(aValue.c_str())) {
			_State.insert(BACK_OFF);
		}
		else if (!FOLLOW_STR.compare(aValue.c_str())) {
			_State.insert(FOLLOW);
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown State Condition: ", aValue);
		}
	}
};
const char* StateCondition::Key = "state";

#endif
