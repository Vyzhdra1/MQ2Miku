#pragma once
#include "AbilityCondition.h"
#include "SpellUtils.h"
#include "BlockedSpellsManager.h"

class BuffExistsCondition : public AbilityCondition {
public:
	inline static const std::string Key = "buffexists";

	BuffExistsCondition(std::set<std::string> aSettings) : AbilityCondition(aSettings) {}

	bool HasBuff(PSPELL aSpell) {
		bool lCanLandBuff = SpellUtils::CanBuffLand(aSpell);
		bool lIsInBuffGroup = SpellUtils::IsBuffInSameGroup(aSpell);
		bool IsSpellBlocked = BlockedSpellsManager::Get()->IsSpellBlocked(aSpell->ID);

		return !lCanLandBuff || lIsInBuffGroup && IsSpellBlocked;
	}

	virtual bool ConditionMet(Ability* aAbility) override {
		if (!aAbility) return false;
		if (!aAbility->GetSpell()) return false;

		bool lHasBuff = HasBuff(aAbility->GetSpell());

		if (!aAbility->HasTriggerSpells()) return lHasBuff == _BooleanCondition;

		lHasBuff = true;

		std::set<PSPELL> lTriggerSpells = aAbility->GetTriggerSpells();
		for (PSPELL lSpell : lTriggerSpells) {
			lHasBuff = lHasBuff && HasBuff(aAbility->GetSpell());
		}
		return lHasBuff == _BooleanCondition;
	}
};