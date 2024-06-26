#pragma once
#include "Ability.h"
#include "SpellUtils.h"
#include "DbSpell.h"

class SpellAbility : public Ability
{
private:
	unsigned long _NextCastTime = 0;
public:
	static const char* ConfigKey;

	SpellAbility() {}

	SpellAbility(DbSpell * aDbSpell) {
		_Spell = SpellUtils::GetSpellInBookByRawData(
			aDbSpell->GetSPA1(),
			aDbSpell->GetSPA2(),
			aDbSpell->GetSPA3(),
			aDbSpell->GetTarget(),
			aDbSpell->GetTimer(),
			aDbSpell->GetHasRecourse()
		);
		SetKey(aDbSpell->GetSpellKey());
		SetName(aDbSpell->GetSpellKey());

		_IsBuff = aDbSpell->GetIsBuff();

		if (_Spell) {
			LoadTriggerSpells();
		}
	}

	virtual bool IsLoaded() override {
		return _Spell;
	}

	MatchType DefaultMatchType() {
		return MatchType::PARTIALMATCH;
	}

	bool AbilityFound() {
		if (!_Spell) {
			_Spell = SpellUtils::GetSpellFromSpellBook(GetKey().c_str(), _MatchType, _LevelMod);
		}

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
		if (!_Spell) {
			return false;
		}

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

	static bool GemReady(DWORD ID) {
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

		sprintf_s(lCommand, "/cast %d", lGemID + 1);
		EzCommand(lCommand);
		_NextCastTime = Utils::GetClockTime() + _Spell->RecastTime;
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Spell->Name);
	}

	virtual void EchoLoadSuccessMessage() {
		std::string lMessage = "Level: " + std::to_string(_Spell->ClassLevel[GetPcProfile()->Class]) + " | Key: " + GetKey() + " | Name: " + _Spell->Name;
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Spell: ", lMessage);
	}

	virtual std::string GetType() {
		return SpellAbility::ConfigKey;
	}
};
const char* SpellAbility::ConfigKey = "spell";
