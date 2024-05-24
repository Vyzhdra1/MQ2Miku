#ifndef ABILITY
#define ABILITY
#include "Utils.h"
#include "SpellUtils.h"
#include <string>
#include <mq/Plugin.h>

class Condition;

class Ability
{
private:
	std::string _Key;
	std::string _Name = "";
protected:
	PSPELL _Spell = 0;
	std::set<PSPELL> _TriggerSpells;
	int _LevelRange = 0;
	int _LevelMod = 0;
	unsigned long _RecastLimit = 15000;
	MatchType _MatchType = DefaultMatchType();
public:	
	virtual void Cast() = 0;
	virtual bool AbilityFound() = 0;
	virtual bool AbilityReady() = 0;
	virtual void EchoCastMessage(char aSuccessType, char * aMessage) = 0;

	void LoadTriggerSpells() {
		if (HasTriggerSpells()) return;

		if (!_Spell) return;

		for (int lIndex = 0; lIndex < GetSpell()->GetNumEffects(); lIndex++) {
			int lSPA = GetSpellAttrib(_Spell, lIndex);

			int lID = (int) GetSpellBase2(_Spell, lIndex);
			if ((lSPA == SPA_CHANCE_BEST_IN_SPELL_GROUP) || (lSPA == SPA_TRIGGER_BEST_IN_SPELL_GROUP)) {
				std::set<PSPELL> lGroupSpells = SpellUtils::GetSpellsBySpellGroupID(lID);
				_TriggerSpells.insert(std::begin(lGroupSpells), std::end(lGroupSpells));
			}
			else if ((lSPA == SPA_TRIGGER_SPELL) || (lSPA == SPA_CHANCE_SPELL)) {
				PSPELL lSpell = GetSpellByID(lID);
				if (!lSpell) continue;
				_TriggerSpells.insert(lSpell);
			}
			else {
				continue;
			}
		}
	}

	bool HasTriggerSpells() {
		return !_TriggerSpells.empty();
	}

	std::set<PSPELL> GetTriggerSpells() {
		return _TriggerSpells;
	}
	
	virtual MatchType DefaultMatchType() {
		return MatchType::PARTIALMATCH;
	}

	virtual bool Memorized() {
		return true;
	}

	PSPELL GetSpell() {
		return _Spell;
	}

	std::string GetName() {
		return _Name;
	}

	void SetName(std::string aName) {
		_Name = aName;
	}

	void SetKey(std::string aKey) {
		_Key = aKey;
	}

	const char * GetKey() {
		return _Key.c_str();
	}

	virtual DWORD GetSpellID() {
		return _Spell ? _Spell->ID : 0;
	}

	void SetModLevel(std::string aModLevel) {
		if (!aModLevel.compare("")) return;

		_LevelMod = Utils::StrToInt(aModLevel.c_str(), -1);
	}

	void SetMatchType(MatchType aMatchType) {
		if (aMatchType == MATCHUNKNOWN) return;

		_MatchType = aMatchType;
	}

	virtual bool CanCastSimultaneously() {
		return false;
	}

	virtual void Refresh() {}

	virtual bool CanCast() {
		return AbilityReady();
	}

	bool IsCasting() {
		return _Spell && Utils::IsCastingThis(GetSpellID());
	}

	bool IsFastCast() {
		return !_Spell || (_Spell->CastTime < (SettingManager::Get()->GetUpdateDelay() * 3));
	}

	virtual bool ShouldBeAbleToRecast() {
		return true;
	}

	virtual void EchoLoadSuccessMessage() = 0;

	virtual std::string GetType() = 0;
};

#endif