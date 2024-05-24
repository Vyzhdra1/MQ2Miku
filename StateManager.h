#ifndef H_StateManager
#define H_StateManager
#include <set>
#include "SettingManager.h"

enum MikuState { STATE_ATTACK, IN_COMBAT, BACK_OFF, FOLLOW, BURN, LIGHT_BURN };

class StateManager {
private:
	std::set<MikuState> _State;
	unsigned long _OOCTime = 0;
	unsigned long _CombatTime = 0;
public:
	StateManager() {
		ClearStates();
	}

	unsigned long GetClockTime() {
		return (unsigned long)clock();
	}

	void ClearStates() {
		_State.clear();
	}

	bool IsStateActive(MikuState aState) {
		return _State.find(aState) != _State.end();
	}

	void ActivateState(MikuState aState) {
		_State.insert(aState);
	}

	void RemoveState(MikuState aState) {
		if (IsStateActive(aState)) {
			_State.erase(aState);
		}
	}

	void ProcessFollowState() {
		if (!IsStateActive(FOLLOW)) {
			return;
		}
		EzCommand("/stick hold uw 10");
		RemoveState(FOLLOW);
		Utils::MikuEcho(Utils::WARNING_COLOR, "Follow State");
	}

	void ProcessBackOffState() {
		if (!IsStateActive(BACK_OFF)) {
			return;
		}

		RemoveState(BACK_OFF);
		Utils::MikuEcho(Utils::WARNING_COLOR, "Backing Off");
	}

	void InitiateBackOff() {
		ActivateState(BACK_OFF);
	}

	void InitiateFollow() {
		ActivateState(BACK_OFF);
		ActivateState(FOLLOW);
	}

	void InitiateBurn() {
		ActivateState(BURN);
	}

	void InitiateAttack() {
		ActivateState(STATE_ATTACK);

		_CombatTime = GetClockTime() + SettingManager::Get()->GetCombatTimer();

		EzCommand("/pet attack");
		EzCommand("/pet swarm");
	}

	bool IsBackOffInitiated() {
		return IsStateActive(BACK_OFF);
	}

	void ProcessAttackState() {
		bool lCanBeAttackState =
			(pTarget) &&
			(pTarget->StandState != STANDSTATE_DEAD) &&
			(GetCharInfo()->pSpawn->StandState != STANDSTATE_FEIGN) &&
			(GetCharInfo()->pSpawn->StandState != STANDSTATE_DEAD) &&
			!IsStateActive(BACK_OFF) &&
			IsStateActive(STATE_ATTACK) &&
			(IsStateActive(IN_COMBAT) || GetClockTime() < _CombatTime || SettingManager::Get()->IsValueMatched(COMBAT_TIMER_ENABLED_STR, OFF_STR));

		if (IsStateActive(STATE_ATTACK)) {
			if (!lCanBeAttackState) {
				RemoveState(STATE_ATTACK);
				InitiateBackOff();
			}
			else {
				MeleeUtils::MonitorStick();
			}
		}
	}

	void ProcessInCombatState() {
		if (PlayerUtils::InCombat() && !IsStateActive(IN_COMBAT)) {
			ActivateState(IN_COMBAT);
		}
		if (!PlayerUtils::InCombat() && IsStateActive(IN_COMBAT)) {
			RemoveState(IN_COMBAT);
		}
	}

	void ProcessBurnState() {
		if (!IsStateActive(STATE_ATTACK)) {
			RemoveState(BURN);
		}
	}

	void ProcessStates() {
		ProcessAttackState();
		ProcessInCombatState();
		ProcessBurnState();
	}

	void FinaliseStateProcess() {
		ProcessBackOffState();
		ProcessFollowState();
	}
};

#endif