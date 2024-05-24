#ifndef H_DISCABILITY
#define H_DISCABILITY
#include "Ability.h"
#include "PlayerUtils.h"

class DiscAbility : public Ability
{
private:
	PALTABILITY _Ability;
public:
	static const char* ConfigKey;

	MatchType DefaultMatchType() {
		return MatchType::PARTIALMATCH;
	}

	bool AbilityFound() {
		if (_MatchType == IDMATCH) {
			int lID = Utils::StrToInt(GetKey(), -1);
			_Spell = GetSpellByID(lID);

			return _Spell != 0;
		}

		_Spell = SpellUtils::GetCombatAbility(GetKey(), _MatchType, _LevelMod);

		return _Spell;
	}

	bool AbilityReady() {
		DWORD timeNow = (DWORD)time(NULL);
		return 
			(pPCData->GetCombatAbilityTimer(_Spell->ReuseTimerIndex, _Spell->SpellGroup) < timeNow) &&
			GetCharInfo()->pSpawn->EnduranceCurrent > _Spell->EnduranceCost;
	}

	virtual bool ShouldBeAbleToRecast() {
		return AbilityReady();
	}

	bool CanCastSimultaneously() {
		return (_Spell && _Spell->CastTime == 0);
	}

	void Cast() {
		pCharData->DoCombatAbility(_Spell->ID);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Spell->Name);
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Discipline: ", _Spell->Name);
	}

	virtual std::string GetType() {
		return DiscAbility::ConfigKey;
	}
};
const char* DiscAbility::ConfigKey = "disc";
#endif