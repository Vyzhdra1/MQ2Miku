#pragma once

#include "Ability.h"
#include "AbilityCollectionAbility.h"
#include "PlayerUtils.h"
#include "AbilityManager.h"
#include "SettingManager.h"
#include <set>

class ScribeSetAction : public AbilityCollectionAbility
{
private:
	std::map<std::string, std::vector<DbSpellSetLink*>> _SpellSets;
	int _MemoriseOffset = 2000;
	unsigned long _MemorizeTimer = Utils::GetClockTime();
public:
	static const char* Key;

	ScribeSetAction() : AbilityCollectionAbility() {
		_SpellSets = *DbManager::Get()->GetSpellSetContext()->GetSpellSets();
	}

	virtual bool CanCastSimultaneously() {
		return false;
	}

	virtual bool HasAbilities() override {
		return true;
	}

	bool ExecuteAbility() {
		if (!ConditionsMet()) return false;

		if (!SettingManager::Get()->IsValueMatched(AUTOSCRIBE_STR, ON_STR)) return false;

		std::string lSpellSetKey = SettingManager::Get()->GetValue(SPELLSET_STR);

		if (lSpellSetKey.empty()) return false;

		if (!_SpellSets.count(lSpellSetKey)) return false;

		std::vector<DbSpellSetLink*> lSpellSet = _SpellSets[lSpellSetKey];

		for (DbSpellSetLink* lSpellLink : lSpellSet) {
			Ability* lAbility = AbilityManager::Get()->GetAbility(lSpellLink->GetSpellKey());

			if (!lAbility) continue;

			if (lAbility->Memorized(true)) continue;

			if (Utils::GetClockTime() < _MemorizeTimer) return true;

			lAbility->Memorize(lSpellLink->GetGemIndex());
			_MemorizeTimer = Utils::GetClockTime() + _MemoriseOffset;
		}
		Reset();
		return false;
	}
};
const char* ScribeSetAction::Key = "scribe";