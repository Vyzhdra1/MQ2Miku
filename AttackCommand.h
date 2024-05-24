#ifndef H_AttackCommand
#define H_AttackCommand
#include "Utils.h"
#include "AssistCommand.h"
#include "MikuPlayer.h"

class AttackCommand
{
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		if (!PlayerUtils::CanAssist(pChar, aInput[CommandConsts::TARGET_KEY], SettingManager::Get()->GetAssistRange())) {
			return false;
		}

		EzCommand("/stick break");

		if (SettingManager::Get()->IsValueMatched(ROLE_STR, HEALER_STR)) return true;

		if (!AssistCommand::Execute(pChar, aInput, aPlayer)) return false;

		int lCommander = 0;
		if (aInput.find(CommandConsts::LEADER_KEY) != aInput.end()) {
			lCommander = Utils::StrToInt(aInput[CommandConsts::LEADER_KEY], 0);
		}

		aPlayer->InitiateAttack(GetCharInfo()->pSpawn, aInput[CommandConsts::TARGET_KEY], lCommander);

		return true;
	}
};
#endif