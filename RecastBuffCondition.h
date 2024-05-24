#ifndef H_RecastBuffCondition
#define H_RecastBuffCondition
#include "Ability.h"
#include "Condition.h"
#include "Utils.h"

class RecastBuffCondition : public Condition
{
private:
	long _TimeRemaining = 4000;
public:
	static const char * Key;

	RecastBuffCondition(std::set<std::string> aSettings) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			_TimeRemaining = Utils::StrToInt((*lIterator).c_str(), -1);

			if (_TimeRemaining < 0) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", (*lIterator));
			}
		}
	}

	bool ConditionMet(Ability * aAbility) {
		PcProfile * lCharInfo = GetPcProfile();
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++)
		{
			if (lCharInfo->GetEffect(nBuff).SpellID == aAbility->GetSpellID())
			{
				return lCharInfo->GetEffect(nBuff).Duration < _TimeRemaining;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++)
		{
			if (lCharInfo->GetTempEffect(nBuff).SpellID == aAbility->GetSpellID())
			{
				return lCharInfo->GetTempEffect(nBuff).Duration < _TimeRemaining;
			}
		}

		if (pAuraMgr) {
			for (int lIndex = 0; lIndex < pAuraMgr->Auras.GetLength(); lIndex++)
			{
				if (!_strnicmp(pAuraMgr->Auras[lIndex].Name, aAbility->GetSpell()->Name, strlen(pAuraMgr->Auras[lIndex].Name))) {
					return false;
				}
			}
		}

		return true;
	}
};

const char * RecastBuffCondition::Key = "recastbuff";

#endif