#pragma once
#include "Utils.h"

class MikuMovementUtils {
private:
	inline static unsigned long _StoppedMovingTimer;
	inline static unsigned long _StoppedTime;
	inline static int _X = 0;
	inline static int _Y = 0;
public:
	static void UpdateLocation() {
		if (!GetCharInfo()) return;
		if (!GetCharInfo()->pSpawn) return;

		if (IsMoving() || !PlayerUtils::IsStanding() || Utils::IsCasting() || PlayerUtils::IsSticking()) {
			_StoppedMovingTimer = Utils::GetClockTime();
			_StoppedTime = 0;
		}
		else {
			_StoppedTime = Utils::GetClockTime() - _StoppedMovingTimer;
		}

		_X = (int) GetCharInfo()->pSpawn->X;
		_Y = (int) GetCharInfo()->pSpawn->Y;
	}

	static bool IsMoving() {
		if (!GetCharInfo()) return false;
		if (!GetCharInfo()->pSpawn) return false;

		return _X != (int)GetCharInfo()->pSpawn->X || _Y != (int)GetCharInfo()->pSpawn->Y;
	}

	static long GetStoppedTime() {
		return _StoppedTime;
	}

	static bool IsUserMoved() {
		return (pEverQuestInfo->keyDown[CMD_FORWARD]
			|| pEverQuestInfo->keyDown[CMD_BACK]
			|| pEverQuestInfo->keyDown[CMD_LEFT]
			|| pEverQuestInfo->keyDown[CMD_RIGHT]
			|| pEverQuestInfo->keyDown[CMD_STRAFE_LEFT]
			|| pEverQuestInfo->keyDown[CMD_STRAFE_RIGHT]
			|| pEverQuestInfo->AutoRun);
	}
};