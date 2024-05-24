#ifndef H_RecastDebuffCondition
#define H_RecastDebuffCondition
#include "Ability.h"
#include "Condition.h"
#include "Utils.h"

class RecastDebuffCondition: public Condition
{
private:
	long _TimeRemaining = 4000;
public:
	static const char * Key;

	RecastDebuffCondition(std::set<std::string> aSettings) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			_TimeRemaining = Utils::StrToInt((*lIterator).c_str(), -1);

			if (_TimeRemaining == -1) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", (*lIterator));
			}
		}
	}

	bool ConditionMet(Ability * aAbility) {
		if (!pTargetWnd) return false;
		for (DWORD i = 0; i < NUM_BUFF_SLOTS; i++) {
			if (pTargetWnd->GetBuff(i) != aAbility->GetSpellID()) continue;
			return pTargetWnd->GetBuffTimer(i) < _TimeRemaining;
		}

		return true;
	}
};

const char * RecastDebuffCondition::Key = "recastdebuff";

#endif