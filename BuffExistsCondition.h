#pragma once
#include "AbilityCondition.h"
#include "SpellUtils.h"

class BuffExistsCondition : public AbilityCondition {
public:
	inline static const std::string Key = "buffexists";

	BuffExistsCondition(std::set<std::string> aSettings) : AbilityCondition(aSettings) {}

	virtual bool ConditionMet(Ability* aAbility) override {
		if (!_Ability) return false;
		if (!_Ability->GetSpell()) return false;

		bool lHasBuff = SpellUtils::HasBuff(_Ability->GetSpellID()) != 0;

		if (!_Ability->HasTriggerSpells()) return lHasBuff == _BooleanCondition;

		std::set<PSPELL> lTriggerSpells = _Ability->GetTriggerSpells();

		for (PSPELL lSpell : lTriggerSpells) {
			lHasBuff = lHasBuff || SpellUtils::HasBuff(lSpell->ID);
		}
		return lHasBuff == _BooleanCondition;
	}
};