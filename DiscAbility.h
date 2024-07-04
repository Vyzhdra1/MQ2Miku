#ifndef H_DISCABILITY
#define H_DISCABILITY
#include "Ability.h"
#include "PlayerUtils.h"

class DiscAbility : public Ability
{
public:
	static const char* ConfigKey;

	DiscAbility() {}

	DiscAbility(DbSpell* aDbSpell) {
		_Spell = SpellUtils::GetCombatAbilityByRawData(
			aDbSpell->GetSPA1(),
			aDbSpell->GetSPA2(),
			aDbSpell->GetSPA3(),
			aDbSpell->GetModLevel(),
			aDbSpell->GetExpansionOffset(),
			aDbSpell->GetPartialName(),
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
		if (_MatchType == IDMATCH) {
			int lID = Utils::StrToInt(GetKey().c_str(), -1);
			_Spell = GetSpellByID(lID);

			return _Spell != 0;
		}

		_Spell = SpellUtils::GetCombatAbility(GetKey().c_str(), _MatchType, _LevelMod);

		return _Spell;
	}

	bool AbilityReady() {
		if (!IsLoaded()) return false;

		DWORD timeNow = (DWORD)time(NULL);
		return 
			_Spell &&
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
		std::string lMessage = "Level: " + std::to_string(_Spell->ClassLevel[GetPcProfile()->Class]) + " | Key: " + GetKey() + " | Name: " + _Spell->Name;
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Spell: ", lMessage);
	}
};
const char* DiscAbility::ConfigKey = "disc";
#endif