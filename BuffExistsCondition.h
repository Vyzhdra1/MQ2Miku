#pragma once
#include "AbilityCondition.h"
#include "SpellUtils.h"
#include "BlockedSpellsManager.h"

class BuffExistsCondition : public AbilityCondition {
private:	
	SpawnType _SpawnType = UNKNOWN;
	int _IntegerValue = 0;

	bool HasBuff(PSPELL aSpell) {
		if (_SpawnType == SpawnType::MY_PET) {
			return HasPetBuff(aSpell);
		}

		if (CanRecastSelfBuff(aSpell, _IntegerValue)) return false;

		bool lCanLandBuff = SpellUtils::CanBuffLand(aSpell);
		if (!lCanLandBuff) return true;

		bool lIsInBuffGroup = SpellUtils::IsBuffInSameGroup(aSpell);
		if (lIsInBuffGroup) return true;

		return BlockedSpellsManager::Get()->IsSpellBlocked(aSpell->ID);
	}

	bool CanRecastSelfBuff(PSPELL aSpell, int aDuration) {
		PcProfile* lCharInfo = GetPcProfile();
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++) {
			if (lCharInfo->GetEffect(nBuff).SpellID == aSpell->ID) {
				return lCharInfo->GetEffect(nBuff).Duration > 0 && lCharInfo->GetEffect(nBuff).Duration < aDuration;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++) {
			if (lCharInfo->GetTempEffect(nBuff).SpellID == aSpell->ID) {
				return lCharInfo->GetEffect(nBuff).Duration > 0 && lCharInfo->GetEffect(nBuff).Duration < aDuration;
			}
		}

		if (pAuraMgr) {
			for (int lIndex = 0; lIndex < pAuraMgr->Auras.GetLength(); lIndex++) {
				if (!_strnicmp(pAuraMgr->Auras[lIndex].Name, aSpell->Name, strlen(pAuraMgr->Auras[lIndex].Name))) return false;
			}
		}

		return false;
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
		int lDuration = Utils::StrToInt(aValue.c_str(), 0);

		if (lDuration) {
			_IntegerValue = lDuration / 6000;
			return;
		}

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