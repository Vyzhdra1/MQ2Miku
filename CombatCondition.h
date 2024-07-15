#ifndef H_CombatConditon
#define H_CombatConditon
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class CombatCondition : public BooleanCondition
{
private:
	bool _InCombat = false;
public:
	static const char * Key;

	CombatCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability * aAbility) {
		return  PlayerUtils::InCombat() == _BooleanCondition;
	}
};

const char * CombatCondition::Key = "combat";

#endif