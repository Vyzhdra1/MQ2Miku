#pragma once
#include "Ability.h"
#include "DbAlternateAbilities.h"

class AAAbility: public Ability
{
private:
	PALTABILITY _Ability = 0;
	int _CastTime = 0;
	int _ActCode = -1;
public:
	static const char* ConfigKey;

	AAAbility() {}

	AAAbility(DbAlternateAbilities* aDbAlternateAbilities) {
		SetKey(std::to_string(aDbAlternateAbilities->GetActCode()));
		SetName(aDbAlternateAbilities->GetAAKey());

		_IsBuff = aDbAlternateAbilities->GetIsBuff();

		_ActCode = aDbAlternateAbilities->GetActCode();
		_Ability = FindAbility();

		if (_Ability) {
			_Spell = GetSpellByID(_Ability->SpellID);
		}

		if (_Spell) {
			_CastTime = _Spell->CastTime;
			LoadTriggerSpells();
		}
	}

	virtual bool IsLoaded() override {
		return _Ability;
	}

	PALTABILITY FindAbility() {
		_ActCode = Utils::StrToInt(GetKey().c_str(), -1);
		for (unsigned long nAbility = 0; nAbility < AA_CHAR_MAX_REAL; nAbility++) {
			PALTABILITY lAbility = pAltAdvManager->GetAAById(pPCData->GetAlternateAbilityId(nAbility));

			if (!lAbility || (lAbility->ID != _ActCode)) continue;

			return lAbility;
		}

		return 0;
	}



	MatchType DefaultMatchType() {
		return MatchType::IDMATCH;
	}

	bool AbilityFound() {
		_ActCode = std::stoi(GetKey());

		for (unsigned long nAbility = 0; nAbility<AA_CHAR_MAX_REAL; nAbility++) {
			if (PALTABILITY lAbility = pAltAdvManager->GetAAById(pPCData->GetAlternateAbilityId(nAbility))) {
				if (lAbility->ID == _ActCode) {
					_Ability = lAbility;
					_Spell = GetSpellByID(_Ability->SpellID);

					if (_Spell) {
						_CastTime = _Spell->CastTime;
					}

					return _Spell;
				}
			}
		}

		return false;
	}
	
	bool AbilityReady() {
		if (!_Ability) {
			return false;
		}

		int lResult = 0;
		if (pAltAdvManager->GetCalculatedTimer(pPCData, _Ability) > 0)
		{
			pAltAdvManager->IsAbilityReady(pPCData, _Ability, &lResult);
		}
		return lResult <= 0;
	}

	virtual bool ShouldBeAbleToRecast() {
		return AbilityReady();
	}

	virtual void Refresh() {
		_Ability = FindAbility();
	}

	bool CanCastSimultaneously() {
		return _CastTime == 0;
	}

	void Cast() {
		char lCommand[MAX_STRING];
		sprintf_s(lCommand, "/alt act %d", _ActCode);
		EzCommand(lCommand);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Ability->GetNameString());
	}

	virtual void EchoLoadSuccessMessage() {
		std::string lMessage = "<Key: " + GetKey() + "> <Name: " + _Ability->GetNameString() + "> ";
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded AA Ability: ", lMessage);
	}

	virtual std::string GetType() {
		return AAAbility::ConfigKey;
	}
};
const char* AAAbility::ConfigKey = "aa";