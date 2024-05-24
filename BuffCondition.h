#pragma once
#include "Ability.h"
#include "SpawnCondition.h"
#include "Utils.h"

class BuffCondition : public SpawnCondition
{
private:
	long _TimeRemaining = 3000;
public:
	static const char* Key;

	BuffCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) {
		bool lResult = false;

		if (_SpawnType == MYTARGET) {
			lResult = TargetCondition();
		}
		else {
			lResult = SelfAssessment();
		}

		if (lResult) {
		}

		return lResult == _BooleanCondition;
	}

	bool SelfAssessment() {
		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++)
		{
			if (GetPcProfile()->GetTempEffect(nBuff).SpellID == _IntegerValue) {
				return true;
			}
		}
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++)
		{
			if (GetPcProfile()->GetEffect(nBuff).SpellID == _IntegerValue) {
				return true;
			}
		}

		return false;
	}

	bool TargetCondition() {
		if (!pTargetWnd) return false;
		for (DWORD i = 0; i < NUM_BUFF_SLOTS; i++) {
			if (pTargetWnd->GetBuff(i) == _IntegerValue) {
				return true;
			}
		}

		return false;
	}
};

const char* BuffCondition::Key = "buff";