#ifndef H_MikuRogue
#define H_MikuRogue
#include "ClassRole.h"
#include "Utils.h"

class MikuRogue : public MeleeRole
{
public:
	virtual void Init() override;
/*	virtual void BurnAction(PSPAWNINFO pChar) override;
	virtual void StartBurnAction(PSPAWNINFO pChar) override;

	void ClearActiveDisc() {
		DiscAbility * lStabbing = new DiscAbility();
		lStabbing->SetKey("Frenzied Stabbing");
		lStabbing->SetLevelRange(-2);
		bool lStabbingFound = lStabbing->AbilityFound();

		DiscAbility * lChance = new DiscAbility();
		lChance->SetLevelRange(-2);
		lChance->SetKey("Twisted Chance");
		bool lChanceFound = lChance->AbilityFound();

		if ((lStabbingFound && Utils::ActiveDiscipline() == lStabbing->GetID()) ||
			(lChanceFound && Utils::ActiveDiscipline() == lChance->GetID())) {
			return;
		}

		EzCommand("/stopdisc");
	}*/
};

void MikuRogue::Init() {
	_ClassName = "ROG";
	MeleeRole::Init();
}
/*
void MikuRogue::BurnAction(PSPAWNINFO pChar) {
	EzCommand("/useitem 23 0"); //Epic
}

void MikuRogue::StartBurnAction(PSPAWNINFO pChar) {
	ClearActiveDisc();
}
*/
#endif