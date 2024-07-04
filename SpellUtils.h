#pragma once
#include "Utils.h"

enum MatchType { MATCHUNKNOWN, PARTIALMATCH, IDMATCH, MODMATCH, EXACTMATCH };

class SpellUtils
{
public:
	static PSPELL GetPlayerSpellById(unsigned long aID) {
		PSPELL lSpell = GetSpellByID(aID);

		if (!lSpell) return 0;

		int lSpellLevel = lSpell->ClassLevel[GetPcProfile()->Class];

		if (lSpell->ClassLevel[GetPcProfile()->Class] > GetPcProfile()->Level) return 0;

		return lSpell;
	}

	static PSPELL GetPlayerSpell(unsigned long aID, const char * aName) {
		PSPELL lSpell = GetPlayerSpellById(aID);

		if (!lSpell) return 0;

		std::string lSpellName(lSpell->Name);
		size_t lMatch = lSpellName.find(aName);
		if (lMatch == std::string::npos) return 0;

		return lSpell;
	}

	static PSPELL GetSpellFromSpellBook(const char* aName, MatchType aMatchType, DWORD aModLevel) {
		PSPELL lResult = 0;
		for (DWORD nSpell = 0; nSpell < NUM_BOOK_SLOTS; nSpell++) {
			if (GetPcProfile()->SpellBook[nSpell] == 0xFFFFFFFF) continue; 

			PSPELL lSpell = SpellUtils::GetPlayerSpell(GetPcProfile()->SpellBook[nSpell], aName);

			if (!lSpell) continue;

			lResult = GetSpell(aName, lSpell, lResult, aMatchType, aModLevel);
		}

		return lResult;
	}

	static PSPELL GetCombatAbility(const char* aName, MatchType aMatchType, DWORD aModLevel) {
		PSPELL lResult = 0;
		for (unsigned long nCombatAbility = 0; nCombatAbility < NUM_COMBAT_ABILITIES; nCombatAbility++) {
			if (!pCombatSkillsSelectWnd->ShouldDisplayThisSkill(nCombatAbility))  continue;

			PSPELL lSpell = SpellUtils::GetPlayerSpell(pLocalPC->GetCombatAbility(nCombatAbility), aName);

			if (!lSpell) continue;

			lResult = GetSpell(aName, lSpell, lResult, aMatchType, aModLevel);
		}

		return lResult;
	}

	static PSPELL GetSpell(const char* aName, PSPELL aFoundSpell, PSPELL aPreviousSpell, MatchType aMatchType, DWORD aModLevel) {
		GetSpellTest(aFoundSpell);
		if ((aMatchType == EXACTMATCH) && strcmp(aName, aFoundSpell->Name)) {
			return aPreviousSpell;
		}

		if (!aPreviousSpell) return aFoundSpell;

		if (aFoundSpell->ClassLevel[GetPcProfile()->Class] < aPreviousSpell->ClassLevel[GetPcProfile()->Class]) {
			return aPreviousSpell;
		}

		if (aMatchType == MatchType::MODMATCH) {
			DWORD lRemainder = aFoundSpell->ClassLevel[GetPcProfile()->Class] % 5;
			if (lRemainder == aModLevel) {
				return aFoundSpell;
			}

			return aPreviousSpell;
		}


		if (aMatchType == MatchType::PARTIALMATCH) {
			if ((!aPreviousSpell) || (aFoundSpell->ClassLevel[GetPcProfile()->Class] > aPreviousSpell->ClassLevel[GetPcProfile()->Class])) {
				return aFoundSpell;
			}
		}

		if (aMatchType == MatchType::MODMATCH) {
			if ((!aPreviousSpell) || (aFoundSpell->ClassLevel[GetPcProfile()->Class] > aPreviousSpell->ClassLevel[GetPcProfile()->Class])) {
				return aFoundSpell;
			}
		}
		return aFoundSpell;
	}

	static PSPELL GetSpellTest(PSPELL aFoundSpell) {
		if (aFoundSpell->GetEffectAttrib(0) == 323) {
			Utils::MikuEcho(Utils::BLUE_COLOR, aFoundSpell->Name);
		}
		return aFoundSpell;
	}

	static PSPELL GetCombatAbilityByRawData(
		int aSPA1,
		int aSPA2,
		int aSPA3,
		int aModLevel,
		int aExpansionOffset,
		std::string aPartialName,
		int aTarget,
		int aTimer,
		bool aHasRecourse
	) {
		PSPELL lResult = 0;
		for (unsigned long lCombatAbility = 0; lCombatAbility < NUM_COMBAT_ABILITIES; lCombatAbility++) {
			if (!pCombatSkillsSelectWnd->ShouldDisplayThisSkill(lCombatAbility)) continue;

			PSPELL lSpell = GetPlayerSpellById(pLocalPC->GetCombatAbility(lCombatAbility));

			if (!IsSpellMatch(aSPA1, aSPA2, aSPA3, aModLevel, aExpansionOffset, aPartialName, aTarget, aTimer, aHasRecourse, lSpell, lResult)) continue;

			lResult = lSpell;
		}

		return lResult;
	}

	static PSPELL GetSpellInBookByRawData(
		int aSPA1,
		int aSPA2,
		int aSPA3,
		int aModLevel,
		int aExpansionOffset,
		std::string aPartialName,
		int aTarget,
		int aTimer,
		bool aHasRecourse
	) {
		PSPELL lResult = 0;
		for (DWORD lSpellIndex = 0; lSpellIndex < NUM_BOOK_SLOTS; lSpellIndex++) {
			if (GetPcProfile()->SpellBook[lSpellIndex] == 0xFFFFFFFF) continue;

			PSPELL lSpell = GetPlayerSpellById(GetPcProfile()->SpellBook[lSpellIndex]);

			if (!IsSpellMatch(aSPA1, aSPA2, aSPA3, aModLevel, aExpansionOffset, aPartialName, aTarget, aTimer, aHasRecourse, lSpell, lResult)) continue;

			lResult = lSpell;
		}

		return lResult;
	}

	static bool IsSpellMatch(
		int aSPA1,
		int aSPA2,
		int aSPA3,
		int aModLevel,
		int aExpansionOffset,
		std::string aPartialName,
		int aTarget,
		int aTimer,
		bool aHasRecourse,
		PSPELL aCurrentSpell,
		PSPELL aPreviousSpell
	) {
		if (!aCurrentSpell) return false;

		if (aPreviousSpell && (aCurrentSpell->ClassLevel[GetPcProfile()->Class] < aPreviousSpell->ClassLevel[GetPcProfile()->Class])) {
			return false;
		}

		if ((aSPA1 > -1) && !IsSPAEffect(aCurrentSpell, aSPA1)) {
			return false;
		}

		if ((aSPA2 > -1) && !IsSPAEffect(aCurrentSpell, aSPA2)) {
			return false;
		}

		if ((aSPA3 > -1) && !IsSPAEffect(aCurrentSpell, aSPA3)) {
			return false;
		}

		if ((aModLevel > -1) && (aCurrentSpell->ClassLevel[GetPcProfile()->Class] % 5 != aModLevel)) {
			return false;
		}

		if ((aExpansionOffset > 0) && (aCurrentSpell->ClassLevel[GetPcProfile()->Class] > (GetPcProfile()->Level - (aExpansionOffset * 5)))) {
			return false;
		}

		if (!aPartialName.empty()) {
			std::string lSpellName(aCurrentSpell->Name);
			if (lSpellName.find(aPartialName) == std::string::npos) return false;
		}

		if (aCurrentSpell->ReuseTimerIndex != aTimer) {
			return false;
		}

		if (aCurrentSpell->TargetType != aTarget) {
			return false;
		}

		if (aHasRecourse && (aCurrentSpell->Autocast == 0)) {
			return false;
		}

		return true;
	}

	static bool IsTriggerSPA(int aSPA)
	{
		int lSPAtoCheck[] = {
			SPA_TRIGGER_BEST_IN_SPELL_GROUP,
			SPA_CHANCE_BEST_IN_SPELL_GROUP,
			SPA_TRIGGER_SPELL,
			SPA_CHANCE_SPELL
		};

		for (int lSPA : lSPAtoCheck)
		{
			if (lSPA == aSPA) return true;
		}

		return false;
	}

	static std::set<PSPELL> GetSpellsBySpellGroupID(int dwSpellGroupID)
	{
		std::set<PSPELL> lResult;
	
		if (pSpellMgr) {
			int* lSpellID = pSpellMgr->SpellGroups.FindFirst(dwSpellGroupID);
			while (lSpellID) {
				PSPELL lSpell = pSpellMgr->GetSpellByID(*lSpellID);
				if (lSpell)	{
					Utils::MikuEcho(Utils::BLUE_COLOR, "Loaded Group Spell: ", lSpell->Name);
					lResult.insert(lSpell);
				}

				lSpellID = pSpellMgr->SpellGroups.FindNext(lSpellID);
			}
		}

		return lResult;
	}

	static long HasBuff(int aSpellID) {
		PcProfile* lCharInfo = GetPcProfile();
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++) {
			if (lCharInfo->GetEffect(nBuff).SpellID == aSpellID)
			{
				return lCharInfo->GetEffect(nBuff).Duration;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++) {
			if (lCharInfo->GetTempEffect(nBuff).SpellID == aSpellID) {
				return lCharInfo->GetTempEffect(nBuff).Duration;
			}
		}
		return 0;
	}

	static bool CanBuffLand(PSPELL aSpell) {
		int lSlotIndex = -1;
		EQ_Affect* lAffect = pLocalPC->FindAffectSlot(aSpell->ID, pLocalPlayer, &lSlotIndex, true, pLocalPC->GetLevel());
		if (lAffect && lSlotIndex != -1)
		{
			if (GetSpellDuration(aSpell, pLocalPlayer->Level, false) >= 1 && lAffect->Duration < 1) {
				return true;
			}
			return false;
		}
		return true;
	}

	static bool IsBuffInSameGroup(PSPELL aSpell) {
		PcProfile* lCharInfo = GetPcProfile();
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++)
		{
			PSPELL lSpell = GetSpellByID(lCharInfo->GetEffect(nBuff).SpellID);

			if ((lSpell) && (lSpell->SpellGroup == aSpell->SpellGroup)) {
				return true;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++)
		{
			PSPELL lSpell = GetSpellByID(lCharInfo->GetTempEffect(nBuff).SpellID);

			if ((lSpell) && (lSpell->SpellGroup == aSpell->SpellGroup)) {
				return true;
			}
		}

		return false;
	}

	/*
	static long HasBuff(int aSpellID) {
		PcProfile* lCharInfo = GetPcProfile();
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++) {
			if (lCharInfo->GetEffect(nBuff).SpellID == aSpellID)
			{
				PSPELL lSpell = GetSpellByID(lCharInfo->GetEffect(nBuff).SpellID);

				if (!lSpell) continue;

				return lCharInfo->GetEffect(nBuff).SpellID;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++) {
			if (lCharInfo->GetTempEffect(nBuff).SpellID == aSpellID) {
				return lCharInfo->GetTempEffect(nBuff).Duration;
			}
		}
		return 0;
	}
	*/
	static bool HasAura(std::string aSpellName) {
		if (pAuraMgr) {
			for (int lIndex = 0; lIndex < pAuraMgr->Auras.GetLength(); lIndex++) {
				if (!_strnicmp(pAuraMgr->Auras[lIndex].Name, aSpellName.c_str(), strlen(pAuraMgr->Auras[lIndex].Name))) {
					return true;
				}
			}
		}
		return false;
	}
};