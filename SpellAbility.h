#ifndef H_SpellAbility
#define H_SpellAbility
#include "Ability.h"
#include "SpellUtils.h"

class SpellAbility : public Ability
{
private:
	unsigned long _NextCastTime = 0;
public:
	static const char* ConfigKey;

	MatchType DefaultMatchType() {
		return MatchType::PARTIALMATCH;
	}

	bool AbilityFound() {
		_Spell = SpellUtils::GetSpellFromSpellBook(GetKey(), _MatchType, _LevelMod);

		return _Spell;
	}

	long FindGemID(DWORD aID)
	{
		for (int GEM = 0; GEM < NUM_SPELL_GEMS; GEM++) {
			if (GetPcProfile()->MemorizedSpells[GEM] == aID) {
				return GEM;
			}
		}
		Utils::MikuEcho(Utils::FAIL_COLOR, "SPELL NOT MEMMED: ", _Spell->Name);
		return -1;
	}

	bool AbilityReady() {
		if (!pCastSpellWnd) return false;
		PcProfile * lPlayer = GetPcProfile();
		long GemID = FindGemID(_Spell->ID);

		if (GemID == -1) return false;

		return 
			(lPlayer->MemorizedSpells[GemID] == _Spell->ID) &&
			GemReady(GemID) &&
			(_Spell->ManaCost < pLocalPlayer->ManaCurrent) &&
			(_Spell->EnduranceCost < pLocalPlayer->ManaCurrent);
	}

	virtual bool ShouldBeAbleToRecast() {
		if (_Spell == 0) true;

		return Utils::GetClockTime() > _NextCastTime;
	}

	static bool GemReady(DWORD ID)
	{
		return
			(pCastSpellWnd && ID < NUM_SPELL_GEMS) &&
			((long)((CCastSpellWnd*)pCastSpellWnd)->SpellSlots[ID]->spellicon != -1) &&
			((long)((CCastSpellWnd*)pCastSpellWnd)->SpellSlots[ID]->spellstate != 1);
	}

	bool Memorized() {
		long lGemID = FindGemID(_Spell->ID);
		return lGemID != -1;
	}

	void Cast() {
		int lGemID = FindGemID(_Spell->ID);

		char lCommand[MAX_STRING];
		/*
		if (GetPcProfile()->Class != Bard) {
			sprintf_s(lCommand, "/casting  \"%s\"|%d -maxtries|5", _Spell->Name, lGemID + 1);
		}
		else {
			sprintf_s(lCommand, "/cast %d", lGemID + 1);
		}
		*/
		sprintf_s(lCommand, "/cast %d", lGemID + 1);
		EzCommand(lCommand);
		_NextCastTime = Utils::GetClockTime() + _Spell->RecastTime;
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Spell->Name);
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Spell: ", _Spell->Name);
	}

	virtual std::string GetType() {
		return SpellAbility::ConfigKey;
	}
};
const char* SpellAbility::ConfigKey = "spell";
#endif