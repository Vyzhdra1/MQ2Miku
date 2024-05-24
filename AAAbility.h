#ifndef H_AAAbility
#define H_AAAbility
#include "Ability.h"

class AAAbility: public Ability
{
private:
	PALTABILITY _Ability;
	int _CastTime;
public:
	static const char* ConfigKey;

	MatchType DefaultMatchType() {
		return MatchType::IDMATCH;
	}

	bool AbilityFound() {
		int lID = std::stoi(GetKey());
		_CastTime = 0;

		for (unsigned long nAbility = 0; nAbility<AA_CHAR_MAX_REAL; nAbility++) {
			if (PALTABILITY lAbility = pAltAdvManager->GetAAById(pPCData->GetAlternateAbilityId(nAbility))) {
				if (lAbility->ID == lID) {
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
		int lResult = 0;
		if (pAltAdvManager->GetCalculatedTimer(pPCData, _Ability) > 0)
		{
			pAltAdvManager->IsAbilityReady(pPCData, _Ability, &lResult);
		}
		DebugSpew("Ability: %d", _Ability);
		DebugSpew("Key: %s", GetKey());
		return lResult <= 0;
	}

	virtual bool ShouldBeAbleToRecast() {
		return AbilityReady();
	}

	virtual void Refresh() {
		AbilityFound();
	}

	bool CanCastSimultaneously() {
		return _CastTime == 0;
	}

	void Cast() {
		char lCommand[MAX_STRING];
		sprintf_s(lCommand, "/alt act %s", GetKey());
		EzCommand(lCommand);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Ability->GetNameString());
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded AA: ", _Spell->Name);
	}

	virtual std::string GetType() {
		return AAAbility::ConfigKey;
	}
};
const char* AAAbility::ConfigKey = "aa";
#endif