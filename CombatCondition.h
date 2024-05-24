#ifndef H_CombatConditon
#define H_CombatConditon
#include "Condition.h"
#include "PlayerUtils.h"

class CombatCondition : public Condition
{
private:
	bool _InCombat = false;
public:
	static const char * Key;

	CombatCondition(std::set<std::string> aSettings) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			if (!strcmp("true", (*lIterator).c_str())) {
				_InCombat = true;
			}
			else if (!strcmp("false", (*lIterator).c_str())) {
				_InCombat = false;
			}
		}
	}

	bool ConditionMet(Ability * aAbility) {
		return  PlayerUtils::InCombat() == _InCombat;
	}
};

const char * CombatCondition::Key = "combat";

#endif