#pragma once
#include <string>
#include "PlayerUtils.h"
#include "SettingManager.h"
#include "MikuMovementUtils.h"
#include <fmt/format.h >

enum MELEEMODE { MELEEMODE_NONE, MELEEMODE_MELEE, MELEEMODE_TANK, MELEEMODE_RANGER };
class MeleeUtils
{
private:
	inline static MELEEMODE MELEE_MODE = MELEEMODE::MELEEMODE_NONE;
	inline static std::string MELEE_STICK_COMMAND = "/stick {} moveback loose !front randomize";
	inline static std::string TANK_STICK_COMMAND = "/stick {} moveback";
	inline static std::string RANGER_STICK_COMMAND = "/stick {} moveback";
	inline static std::string NONE_COMMAND = "/stick";
	inline static unsigned long _LastUserMovement = 0;
	inline static int _FailedRangeCount = 0;
public:
	static long GetRangeOffset() {
		long lDistance = ((PSPAWNINFO)pTarget)->AvatarHeight + 9.0f - (IsMobFleeing(pControlledPlayer, pTarget) * 3.0f);
		switch (MELEE_MODE) {
		case MELEEMODE_NONE: return 0;
		case MELEEMODE_MELEE: return lDistance;
		case MELEEMODE_TANK: return lDistance;
		case MELEEMODE_RANGER: return 35;
		}


		return 0;
	}
	static void BackOff() {
		if (pEverQuestInfo->bAutoRangeAttack) {
			EzCommand("/autofire off");
		}
		if (pEverQuestInfo->bAutoAttack) {
			EzCommand("/attack off");
		}
		_FailedRangeCount = 0;
	}

	static void CommandAttack() {
		EzCommand("/stand");
		if (MELEE_MODE == MELEEMODE_RANGER) {
			if (pEverQuestInfo->bAutoAttack) {
				EzCommand("/attack off");
			}
			EzCommand("/autofire on");
			return;
		}

		if (pEverQuestInfo->bAutoRangeAttack) {
			EzCommand("/autofire off");
		}
		EzCommand("/attack on");
	}

	static std::string GetStick() {
		switch (MELEE_MODE) {
		case MELEEMODE_NONE: return NONE_COMMAND;
		case MELEEMODE_MELEE: return MELEE_STICK_COMMAND;
		case MELEEMODE_TANK: return TANK_STICK_COMMAND;
		case MELEEMODE_RANGER: return RANGER_STICK_COMMAND;
		}

		return NONE_COMMAND;
	}

	static void Reset() {

	}

	static MELEEMODE EvaluateMode() {
		if (!pTarget) return MELEEMODE::MELEEMODE_NONE;

		//if (pAggroInfo && (pAggroInfo->aggroData[AD_Player].AggroPct >= 100))
		
		if (SettingManager::Get()->IsValueMatched(ROLE_STR, TANK_STR)) {
			if ((SettingManager::Get()->GetInt(USER_DELAY_STR) < GetLastUserMovementElapsed()) && pAggroInfo && (pAggroInfo->aggroData[AD_Player].AggroPct < 100)) {
				return MELEEMODE::MELEEMODE_MELEE;
			}
			else {
				return MELEEMODE::MELEEMODE_TANK;
			}
		}

		if (pTarget->TargetOfTarget && (pTarget->TargetOfTarget == GetCharInfo()->pSpawn->SpawnID)) {
			return MELEEMODE::MELEEMODE_TANK;
		}

		if (SettingManager::Get()->IsValueMatched(ROLE_STR, DPS_STR)) {
			if ((GetPcProfile()->Class == Ranger) && (_FailedRangeCount < 3)){
				return MELEEMODE::MELEEMODE_RANGER;
			}

			return MELEEMODE::MELEEMODE_MELEE;
		}

		return MELEEMODE::MELEEMODE_NONE;
	}

	static void IncrementFailedRangeCount() {
		_FailedRangeCount++;
	}

	static void ExecuteCommand() {
		if (MELEE_MODE == MELEEMODE::MELEEMODE_NONE) {
			return;
		}
		long lMeleeRange = GetRangeOffset();
		std::string lCommand = fmt::format(GetStick(), std::to_string(lMeleeRange));

		Utils::MikuEcho(Utils::BLUE_COLOR, "Stick Command: ", lCommand);
		EzCommand(lCommand.c_str());
		CommandAttack();
	}

	static unsigned long GetLastUserMovementElapsed() {
		return Utils::GetClockTime() - _LastUserMovement;
	}

	static void InitiateAttack(int aCommander) {
		MELEE_MODE = EvaluateMode();
		ExecuteCommand();

		//If you are target > go front 
		//If you are tank > check movement
		// -- if old movement > dps mode
		// -- if new movement > tank mode
		//if you are non ranger dps > go back
		//if you are ranger dps > go archery
		// if you cant go archery (how?) > go melee

	}

	static void MonitorStick() {
		if (SettingManager::Get()->GetInt(USER_DELAY_STR) < GetLastUserMovementElapsed()) {
			return;
		}

		MELEEMODE lNewMode = EvaluateMode();
		if (lNewMode != MELEE_MODE) {
			MELEE_MODE = lNewMode;
			ExecuteCommand();
		}
	}

	static void MonitorKeyPress() {
		if (!_LastUserMovement || MikuMovementUtils::IsUserMoved()) {
			_LastUserMovement = Utils::GetClockTime();
		}
	}
};