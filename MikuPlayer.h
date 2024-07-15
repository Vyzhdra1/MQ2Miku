#pragma once
#include "Utils.h"
#include "PlayerUtils.h"
#include "MeleeUtils.h"
#include "StateManager.h"
#include "ActionManager.h"
#include "SpawnManager.h"
#include <mq/Plugin.h>

class MikuPlayer
{
protected:
	bool _IsPetClass = false;
	unsigned long _OOCTime = 0;
	unsigned long _CombatTime = 0;
	unsigned long _TargetID = 0;

	std::string _Role = "";
	bool _IsMelee = true;

	virtual void InitClass() = 0;
public:
	MikuPlayer() {}

	void Init() {
		InitClass();
	}

	void InitiateBurn(PSPAWNINFO pChar, int aTarget, int aCommander) {
		StateManager::Get()->InitiateBurn();
		InitiateAttack(pChar, aTarget, aCommander);
	}

	void BackOff() {
		if (!StateManager::Get()->IsStateActive(BACK_OFF)) {
			return;
		}

		MeleeUtils::BackOff();

		if (_IsPetClass) {
			EzCommand("/pet back");
		}
		if (!StateManager::Get()->IsStateActive(FOLLOW)) {
			EzCommand("/stick off");
		}
		EzCommand("/stopsong");

		MeleeUtils::BackOff();
		MeleeUtils::BackOff();

		ActionManager::Get()->ClearActiveAbilities();
	}

	void InitiateAttack(PSPAWNINFO pChar, int aTarget, int aCommander) {
		if (!PlayerUtils::CanExecuteAbility()) return;

		SpawnManager::Get()->SetAttackTarget(aTarget);

		Utils::MikuEcho(Utils::BLUE_COLOR, "Commencing attack!");
		EzCommand("/stand");

		Utils::TargetByID(pChar, aTarget);

		if (_IsMelee) {
			MeleeUtils::InitiateAttack(aCommander);
		}
		StateManager::Get()->InitiateAttack();
	}

	void InitiateBackOff() {
		SpawnManager::Get()->SetAttackTarget(-1);
		StateManager::Get()->InitiateBackOff();
	}

	bool IsBackOffInitiated() {
		return StateManager::Get()->IsBackOffInitiated();
	}

	void InitiateFollow(PSPAWNINFO pChar, char* aTarget) {
		StateManager::Get()->InitiateFollow();

		Utils::TargetByID(pChar, aTarget);
	}

	void ProcessAutoAttack() {
		bool lIsFakeCombat = SettingManager::Get()->IsValueMatched(FAKE_COMBAT_STR, ON_STR);
		if ((pEverQuestInfo->bAutoRangeAttack || pEverQuestInfo->bAutoAttack) && !StateManager::Get()->IsStateActive(STATE_ATTACK) && pTarget && ((StateManager::Get()->IsStateActive(IN_COMBAT) && _IsMelee) || lIsFakeCombat)) {
			InitiateAttack(pControlledPlayer, pTarget->SpawnID, pControlledPlayer->SpawnID);
		}

		if (!((pEverQuestInfo->bAutoRangeAttack || pEverQuestInfo->bAutoAttack) || !pTarget) && StateManager::Get()->IsStateActive(STATE_ATTACK) && _IsMelee && !lIsFakeCombat) {
			InitiateBackOff();
		}
	}

	virtual void Pulse() {
		ProcessAutoAttack();

		StateManager::Get()->ProcessStates();

		BackOff();

		StateManager::Get()->FinaliseStateProcess();

		ActionManager::Get()->ExecuteAbilities();
	}

	void ParseChat(const char* Line, DWORD Color) {
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
		if (StateManager::Get()->IsStateActive(MikuState::STATE_ATTACK)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in STATE_ATTACK");
		}
		if (StateManager::Get()->IsStateActive(MikuState::IN_COMBAT)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in IN_COMBAT");
		}
		if (StateManager::Get()->IsStateActive(MikuState::BACK_OFF)) {
			Utils::MikuEcho(Utils::BLUE_COLOR, "I am in BACK_OFF");
		}
	}
};