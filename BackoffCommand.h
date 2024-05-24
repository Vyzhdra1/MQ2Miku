#ifndef H_BackoffCommand
#define H_BackoffCommand
#include "CommandConsts.h"
#include "MikuPlayer.h"

class BackoffCommand
{
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		aPlayer->InitiateBackOff();
		return true;
	}
};
#endif