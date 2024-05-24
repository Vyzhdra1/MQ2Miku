#ifndef H_FollowCommand
#define H_FollowCommand
#include "Utils.h"
#include "CommandConsts.h"
#include "AssistCommand.h"
#include "MikuPlayer.h"

class FollowCommand
{
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		if (aInput.find(CommandConsts::LEADER_KEY) == aInput.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No target to target.");
			return false;
		}
		
		if (!Utils::IsTargetInRange(pChar, aInput[CommandConsts::LEADER_KEY], SettingManager::Get()->GetAssistRange())) {
			return false;
		}
		aPlayer->InitiateFollow(pChar, aInput[CommandConsts::LEADER_KEY]);

		return true;
	}
};
#endif