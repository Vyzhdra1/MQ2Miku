#pragma once
#include "Utils.h"
#include "AssistCommand.h"
#include "MikuPlayer.h"
#include "ClassSettings.h"

class BurnCommand
{
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer* aPlayer) {
		if (!Utils::IsTargetInRange(pChar, aInput[CommandConsts::TARGET_KEY], SettingManager::Get()->GetAssistRange())) {
			return false;
		}

		EzCommand("/stick break");

		if (SettingManager::Get()->IsValueMatched(ROLE_STR, HEALER_STR)) return true;

		if (!AssistCommand::Execute(pChar, aInput, aPlayer)) return false;

		int lCommander = 0;
		if (aInput.find(CommandConsts::LEADER_KEY) != aInput.end()) {
			lCommander = Utils::StrToInt(aInput[CommandConsts::LEADER_KEY], 0);
		}

		int aTarget = Utils::StrToInt(aInput[CommandConsts::TARGET_KEY], -1);

		aPlayer->InitiateBurn(GetCharInfo()->pSpawn, aTarget, lCommander);

		return true;
	}
};