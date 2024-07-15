#pragma once
#include "AbilityCondition.h"
#include "SpellUtils.h"
#include "BlockedSpellsManager.h"

class BuffExistsCondition : public AbilityCondition {
private:	
	SpawnType _SpawnType = UNKNOWN;

	bool HasBuff(PSPELL aSpell) {
		if (_SpawnType == SpawnType::MY_PET) {
			return HasPetBuff(aSpell);
		}

		bool lCanLandBuff = SpellUtils::CanBuffLand(aSpell);
		bool lIsInBuffGroup = SpellUtils::IsBuffInSameGroup(aSpell);
		bool IsSpellBlocked = BlockedSpellsManager::Get()->IsSpellBlocked(aSpell->ID);

		return !lCanLandBuff || lIsInBuffGroup || IsSpellBlocked;
	}

	bool HasPetBuff(PSPELL aSpell) {
		bool lHasBuff = SpellUtils::HasPetBuff(aSpell);
		bool IsSpellBlocked = BlockedSpellsManager::Get()->IsSpellBlockedPet(aSpell->ID);

		return lHasBuff || IsSpellBlocked;
	}
public:
	inline static const std::string Key = "buffexists";

	BuffExistsCondition(std::set<std::string> aSettings) : AbilityCondition() {
		Load(aSettings);
	}

	void ParseNextValue(std::string aValue) override {
		if (_SpawnType == UNKNOWN) {
			_SpawnType = PlayerUtils::StringToSpawnType(aValue);
		}

		if (_SpawnType == UNKNOWN) {
			AbilityCondition::ParseNextValue(aValue);
		}
	}

	virtual bool ConditionMet(Ability* aAbility) override {
		if (!aAbility) return false;
		if (!aAbility->GetSpell()) return false;

		bool lHasBuff = HasBuff(aAbility->GetSpell());

		if (!aAbility->HasTriggerSpells()) return lHasBuff == _BooleanCondition;

		lHasBuff = true;

		std::set<PSPELL> lTriggerSpells = aAbility->GetTriggerSpells();
		for (PSPELL lSpell : lTriggerSpells) {
			lHasBuff = lHasBuff && HasBuff(lSpell);
		}
		return lHasBuff == _BooleanCondition;
	}
};