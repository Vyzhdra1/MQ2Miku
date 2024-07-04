#pragma once
#include "Utils.h"

class MikuMovementUtils {
private:
	inline static unsigned long _LastMoveTime = Utils::GetClockTime();
	inline static unsigned long _LastSitTime = Utils::GetClockTime();
	inline static long _TimeSinceLastMove = 0;
	inline static long _TimeSinceLastSit = 0;
	inline static int _X = 0;
	inline static int _Y = 0;
public:
	static void UpdateLocation() {
		if (!GetCharInfo()) return;
		if (!GetCharInfo()->pSpawn) return;

		unsigned long lNow = Utils::GetClockTime();;

		if (IsMoving() || Utils::IsCasting() || PlayerUtils::IsSticking()) {
			_LastMoveTime = lNow;
		}

		if (!PlayerUtils::IsStanding()) {
			_LastSitTime = lNow;
		}

		_TimeSinceLastMove = lNow - _LastMoveTime;
		_TimeSinceLastSit = lNow - _LastSitTime;

		_X = (int) GetCharInfo()->pSpawn->X;
		_Y = (int) GetCharInfo()->pSpawn->Y;
	}

	static bool IsMoving() {
		if (!GetCharInfo()) return false;
		if (!GetCharInfo()->pSpawn) return false;

		return _X != (int)GetCharInfo()->pSpawn->X || _Y != (int)GetCharInfo()->pSpawn->Y;
	}

	static long GetTimeSinceLastMove() {
		return _TimeSinceLastMove;
	}

	static long GetTimeSinceLastSit() {
		return _TimeSinceLastSit;
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