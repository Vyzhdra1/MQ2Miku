#pragma once
#include "AbilityCollectionAbility.h"

class BuffAbilities : public AbilityCollectionAbility
{
private:
	enum BuffState{MEMORIZE, CASTING, NONE, BEGIN, RESETSPELLS, MEMORIZEORIGINAL, COMPLETE};
	const int MAX_RECOVERY_TIME = 5000;
	const int TIMEOUT = 8;
	BuffState _BuffState = NONE;
	std::string _MemmedSpellsCommand;
	const int _StartGemIDX = 3;
	std::vector<Ability*> _OriginalSpells;
	std::vector<Ability*> _BuffAbilities;
	int _TimeoutIteration = 0;

	virtual ~BuffAbilities() {
		DeleteOriginalSpells();
	}

	void DeleteOriginalSpells() {
		for (std::vector<Ability*>::iterator lIterator = _OriginalSpells.begin(); lIterator != _OriginalSpells.end(); lIterator++) {
			delete (*lIterator);
		}

		_OriginalSpells.clear();
	}

	void RefreshBuffAbilities() {
		_BuffAbilities.clear();
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			_BuffAbilities.push_back((*lIterator)->GetAbility());
		}
	}

	void SaveMemmedSpells() {
		DeleteOriginalSpells();

		_MemmedSpellsCommand = "/memorize";
		for (int GEM = 0; GEM < NUM_SPELL_GEMS; GEM++) {
			if (GetPcProfile()->MemorizedSpells[GEM] != -1) {
				SpellAbility * lSpellAbility = new SpellAbility();
				PSPELL lSpell = GetSpellByID(GetPcProfile()->MemorizedSpells[GEM]);
				lSpellAbility->SetKey(lSpell->Name);
				lSpellAbility->SetMatchType(MatchType::EXACTMATCH);
				lSpellAbility->AbilityFound();
				_OriginalSpells.push_back(lSpellAbility);
			}
		}
	}

	void Finalise() {
		DeleteOriginalSpells();
		Reset();
		_BuffState = NONE;

		EzCommand("/pet leader");
		EzCommand("/autoinventory");
		EzCommand("/autoinventory");

		Utils::MikuEcho('g', "Buffing Complete!");
	}

	int GetNextGemSlot(int aCurrentGemSlot) {
		return (aCurrentGemSlot > NUM_SPELL_GEMS ? 1 : ++aCurrentGemSlot);
	}

	void MemorizeAll(std::vector<Ability*> pAbilities, BuffState pNextState) {
		EzCommand("/autoinventory");
		EzCommand("/autoinventory");
		EzCommand("/autoinventory");
		EzCommand("/target myself");		

		int Index = 1;
		std::string lMemorizeCommand = "/memorize";
		for (std::vector<Ability*>::iterator lIterator = pAbilities.begin(); lIterator != pAbilities.end(); lIterator++) {
			if (!(*lIterator)->Memorized()) {
				lMemorizeCommand = lMemorizeCommand + " \"" + std::to_string((*lIterator)->GetSpellID()) + "|" + std::to_string(Index) + "\"";
			}
			Index = GetNextGemSlot(Index);
		}
		Utils::MikuSendCommand(lMemorizeCommand);
		_BuffState = pNextState;
	}

	bool CheckAllMemorized(std::vector<Ability*> pAbilities, BuffState pNextState) {
		for (std::vector<Ability*>::iterator lIterator = pAbilities.begin(); lIterator != pAbilities.end(); lIterator++) {
			if (!(*lIterator)->Memorized()) {
				if (!((pSpellBookWnd && (CSidlScreenWnd*)pSpellBookWnd->dShow))) {
					MemorizeAll(pAbilities, _BuffState);
				}
				return false;
			}
		}
		_BuffState = pNextState;

		return true;
	}

	void AllHasCast() {
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (!(*lIterator)->IsCastSuccessful() && (*lIterator)->ConditionsMet() && (*lIterator)->ShouldBeAbleToRecast()) {
				return;
			}
		}
		_BuffState = RESETSPELLS;
	}

	void CastAbility() {
		bool lAllHasCast = true;
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (!(*lIterator)->IsCastSuccessful() && (*lIterator)->ConditionsMet() && (*lIterator)->ShouldBeAbleToRecast()) {
				if ((*lIterator)->ExecuteAbility()) {
					return;
				}
				lAllHasCast = false;
			}
		}

		if (lAllHasCast) {
			_BuffState = RESETSPELLS;
		}
	}
public:
	static const char* Key;

	bool ExecuteAbility() {
		if (_BuffState == NONE) {
			return false;
		}

		else if (_BuffState == BEGIN) {
			RefreshBuffAbilities();
			SaveMemmedSpells();
			_TimeoutIteration = 0;
			MemorizeAll(_BuffAbilities, MEMORIZE);
		}

		else if (_BuffState == MEMORIZE) {
			CheckAllMemorized(_BuffAbilities, CASTING);
		}

		else if (_BuffState == CASTING) {
			CastAbility();
		}

		else if (_BuffState == RESETSPELLS) {
			if (_TimeoutIteration < TIMEOUT) {
				_TimeoutIteration++;
				return false;
			}
			MemorizeAll(_OriginalSpells, MEMORIZEORIGINAL);
		}

		else if (_BuffState == MEMORIZEORIGINAL) {
			CheckAllMemorized(_OriginalSpells, COMPLETE);
		}

		else if (_BuffState == COMPLETE) {
			Finalise();
		}

		return false;
	}

	bool IsEnabled() {
		return _BuffState != NONE;
	}

	void Enable() {
		_BuffState = BEGIN;
	}

	void Disable() {
		_BuffState = NONE;
	}
};
const char* BuffAbilities::Key = "buff";