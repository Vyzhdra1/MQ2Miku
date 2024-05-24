#pragma once
#include "PlayerUtils.h"
#include "BooleanCondition.h"

class BannedTargetCondition : public BooleanCondition
{
private:
	std::set<std::string> BANNED_NAME = { "#Melalafen00", "#Grolik00" };
public:
	inline static const char* Key = "bannedtarget";

	BannedTargetCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		if (!pTarget) return true;

		if (!pTarget->SpawnID) return true;

		PlayerClient* lSpawn = GetSpawnByID(pTarget->SpawnID);

		if (!lSpawn) return true;

		std::string lName = std::string(lSpawn->Name);

		return (BANNED_NAME.find(lName) != BANNED_NAME.end()) == _BooleanCondition;
	}
};