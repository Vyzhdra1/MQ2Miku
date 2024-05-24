#ifndef H_ASSISTCOMMAND
#define H_ASSISTCOMMAND
#include "Utils.h"
#include "AssistCommand.h"
#include "MikuPlayer.h"
#include "SettingManager.h"

class AssistCommand
{
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		if (!PlayerUtils::CanAssist(pChar, aInput[CommandConsts::TARGET_KEY], SettingManager::Get()->GetAssistRange())) {
			return false;
		}

		char lCommand[MAX_STRING];
		sprintf_s(lCommand, "/target id %s", aInput[CommandConsts::TARGET_KEY]);
		EzCommand(lCommand);

		return true;
	}
};
#endif