#ifndef H_BuffActiveCondition
#define H_BuffActiveCondition
#include "SpellAbility.h"
#include "Ability.h"
#include "BooleanCondition.h"
#include "Utils.h"
#include <set>

class BuffActiveCondition : public BooleanCondition
{
private:
	int _ActiveBuffID = -1;
public:
	static const char * Key;

	BuffActiveCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability * aAbility) {
		for (unsigned long nBuff = 0; nBuff<NUM_SHORT_BUFFS; nBuff++)
		{
			if (GetPcProfile()->GetTempEffect(nBuff).SpellID == _ActiveBuffID) {
				return _BooleanCondition == true;
			}
		}
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++)
		{
			if (GetPcProfile()->GetEffect(nBuff).SpellID == _ActiveBuffID) {
				return _BooleanCondition == true;
			}
		}

		return _BooleanCondition == false;
	}

	virtual void ParseNextValue(std::string aValue) override {
		_ActiveBuffID = Utils::StrToInt(aValue.c_str(), -1);

		if (_ActiveBuffID == -1) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
		}
	}
};

const char * BuffActiveCondition::Key = "buffactive";

#endif