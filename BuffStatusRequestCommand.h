#pragma once
#include "Utils.h"
#include "SpellUtils.h"
#include "CommandConsts.h"
#include "MikuPlayer.h"
class BuffStatusRequestCommand {
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer* aPlayer) {
		if (aInput.find(CommandConsts::ABILITYSET_KEY) == aInput.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No abilities found");
			return false;
		}

		std::stringstream lBuffsToCheck(aInput[CommandConsts::ABILITYSET_KEY]);
		std::string lBuffID;
		std::set<int> lBuffIDs;

		while (std::getline(lBuffsToCheck, lBuffID, ',')) {
			int lIntBuffID = Utils::StrToInt(lBuffID.c_str(), 0);

			if (!lIntBuffID) continue;

			lBuffIDs.insert(lIntBuffID);
		}

		if (!lResponse.compare("")) return false;

		int lSpawnID = Utils::StrToInt(aInput[CommandConsts::LEADER_KEY], 0);

		if (!lSpawnID) return false;

		PlayerClient* lSpawn = GetSpawnByID(lSpawnID);

		if (!lSpawn) return false;

		Utils::MikuSendCommand("/mikubc c=buffreq;a=" + lResponse + "b=dex " + lSpawn->Name);

		return true;
	}
};