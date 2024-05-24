#ifndef H_SpellUtils
#define H_SpellUtils
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
#endif