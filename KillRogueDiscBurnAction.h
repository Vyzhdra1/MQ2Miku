#ifndef H_KillRogueDiscBurnAction
#define H_KillRogueDiscBurnAction
#include "Ability.h"
#include "DiscAbility.h"

class KillRogueDiscBurnAction: public Ability
{
private:
	DiscAbility * _Stabbing = 0;
	DiscAbility * _Chance = 0;
	DiscAbility * _RaggedEdge = 0;
public:
	~KillRogueDiscBurnAction() {
		if (_Stabbing) delete _Stabbing;
		if (_Chance) delete _Chance;
		if (_RaggedEdge) delete _RaggedEdge;
	}

	virtual bool IsLoaded() override {
		return AbilityFound();
	}

	virtual bool AbilityFound() {
		_Stabbing = GetDisc("Frenzied Stabbing");
		_Chance = GetDisc("Twisted Chance");
		_RaggedEdge = GetDisc("Ragged Edge");

		return _Stabbing || _Chance || _RaggedEdge;
	}

	DiscAbility* GetDisc(std::string aDiscName) {
		DiscAbility * lDisc = new DiscAbility();
		lDisc->SetKey(aDiscName);

		if (!lDisc->AbilityFound()) {
			delete lDisc;
			return 0;
		}
		else {
			return lDisc;
		}
	}

	virtual void Cast() {
		EzCommand("/stopdisc");
	}

	bool DiscReady(DiscAbility * aDisc) {
		return aDisc && aDisc->AbilityReady();
	}

	bool DiscActive(DiscAbility * aDisc) {
		return aDisc && Utils::ActiveDiscipline() == aDisc->GetSpellID();
	}

	virtual bool AbilityReady() {
		return
			(DiscReady(_Stabbing) || DiscReady(_Chance) || DiscReady(_RaggedEdge)) &&
			!(DiscActive(_Stabbing) || DiscActive(_Chance) || DiscActive(_RaggedEdge));
	}

	virtual void EchoCastMessage(char aSuccessType, char * aMessage){
		Utils::MikuEcho(aSuccessType, "Cancelling active discipline!");
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded: ", "Rogue Burn Disc");
	}

	virtual std::string GetType() {
		return "Rogue";
	}
};
#endif