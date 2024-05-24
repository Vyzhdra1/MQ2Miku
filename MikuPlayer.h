#ifndef MIKUPLAYER
#define MIKUPLAYER
#include "Utils.h"
#include "PlayerUtils.h"
#include "MeleeUtils.h"
#include "StateManager.h"
#include "ActionManager.h"
#include "GameManager.h"

class MikuPlayer
{
protected:
	char* _ClassName;
	StateManager* _StateManager;
	ActionManager* _ActionManager;

	bool _IsPetClass = false;
	unsigned long _OOCTime = 0;
	unsigned long _CombatTime = 0;

	std::string _Role = "";
	bool _IsMelee = true;

	virtual void InitClass() = 0;
public:
	ActionManager* GetActionManager() {
		return _ActionManager;
	}

	~MikuPlayer() {
		delete _ActionManager;
	}

	void Init() {
		_ActionManager = new ActionManager();
		_StateManager = GameManager::GetStateManager();

		InitClass();
	}

	char* ClassName() {
		return _ClassName;
	}

	void InitiateBurn(PSPAWNINFO pChar, char* aTarget, int aCommander) {
		_StateManager->InitiateBurn();
		InitiateAttack(pChar, aTarget, aCommander);
	}

	void BackOff() {
		if (!_StateManager->IsStateActive(BACK_OFF)) {
			return;
		}

		MeleeUtils::BackOff();

		if (_IsPetClass) {
			EzCommand("/pet back");
		}
		if (!_StateManager->IsStateActive(FOLLOW)) {
			EzCommand("/stick off");
		}
		EzCommand("/stopsong");

		MeleeUtils::BackOff();
		MeleeUtils::BackOff();

		_ActionManager->ClearActiveAbilities();
	}

	void InitiateAttack(PSPAWNINFO pChar, char* aTarget, int aCommander) {
		if (!PlayerUtils::CanExecuteAbility()) return;

		Utils::MikuEcho(Utils::BLUE_COLOR, "Commencing attack!");
		EzCommand("/stand");

		Utils::TargetByID(pChar, aTarget);

		if (_IsMelee) {
		//	EzCommand("/killthis");
		//	EzCommand("/attack on");
			MeleeUtils::InitiateAttack(aCommander);
		}
		_StateManager->InitiateAttack();
	}

	void InitiateBackOff() {
		_StateManager->InitiateBackOff();
	}

	bool IsBackOffInitiated() {
		return _StateManager->IsBackOffInitiated();
	}

	void InitiateFollow(PSPAWNINFO pChar, char* aTarget) {
		_StateManager->InitiateFollow();

		Utils::TargetByID(pChar, aTarget);
	}

	void ProcessAutoAttack() {
		if ((pEverQuestInfo->bAutoRangeAttack || pEverQuestInfo->bAutoAttack) && !_StateManager->IsStateActive(STATE_ATTACK) && _StateManager->IsStateActive(IN_COMBAT) && pTarget && _IsMelee) {
			char* lTargetID = (char*) std::to_string(pTarget->SpawnID).c_str();
			InitiateAttack(pControlledPlayer, lTargetID, pControlledPlayer->SpawnID);
		}

		if (!((pEverQuestInfo->bAutoRangeAttack || pEverQuestInfo->bAutoAttack) || !pTarget) && _StateManager->IsStateActive(STATE_ATTACK) && _IsMelee) {
			InitiateBackOff();
		}
	}

	void Pulse() {
		ProcessAutoAttack();

		_StateManager->ProcessStates();

		BackOff();

		_StateManager->FinaliseStateProcess();

		_ActionManager->ExecuteAbilities();
	}

	void ParseChat(const char* Line, DWORD Color) {
	/*	if (!strcmp(Line, "Auto attack is on.")) {
			InitiateAttack(GetCharInfo()->pSpawn, "", 0);
		}
		if (!strcmp(Line, "Auto fire on.")) {
			InitiateAttack(GetCharInfo()->pSpawn, "", 0);
		}
		if (!strcmp(Line, "Auto fire off.")) {
			InitiateBackOff();
		}
		if (!strcmp(Line, "Auto attack is off.")) {
			InitiateBackOff();
		}*/
		if (!strcmp(Line, "You escape from combat and evade your opponents.")) {
			InitiateBackOff();
		}
		if (!strcmp(Line, "Your target is too close to use a ranged weapon!")) {
			MeleeUtils::IncrementFailedRangeCount();
		}
		else if (!strcmp(Line, "You are now the leader of your group.")) {
			EzCommand("/grouproles set Albedo 1");
			EzCommand("/grouproles set Albedo 2");
			EzCommand("/grouproles set Albedo 3");
			EzCommand("/grouproles set Moriknight 1");
			EzCommand("/grouproles set Moriknight 2");
			EzCommand("/grouproles set Moriknight 3");
			EzCommand("/grouproles set Soullknight 1");
			EzCommand("/grouproles set Soullknight 2");
			EzCommand("/grouproles set Soullknight 3");
		}
	}

	void Report() {
		if (_StateManager->IsStateActive(MikuState::STATE_ATTACK)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in STATE_ATTACK");
		}
		if (_StateManager->IsStateActive(MikuState::IN_COMBAT)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in IN_COMBAT");
		}
		if (_StateManager->IsStateActive(MikuState::BACK_OFF)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in BACK_OFF");
		}
	}

	std::string GetRole() {
		return _Role;
	}
};
#endif