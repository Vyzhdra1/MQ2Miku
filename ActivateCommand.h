#ifndef H_ActivateCommand
#define H_ActivateCommand
#include "Utils.h"
#include "CommandConsts.h"
#include "MikuPlayer.h"

class ActivateCommand
{

public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		if (aInput.find(CommandConsts::ABILITYSET_KEY) == aInput.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No ability to activate");
			return false;
		}

		ActionManager::Get()->ActivateAbility(aInput[CommandConsts::ABILITYSET_KEY]);
		return true;
	}
};
#endif