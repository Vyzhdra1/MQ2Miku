#ifndef H_MelodyAbility
#define H_MelodyAbility
#include "Ability.h"

class MelodyAbility : public Ability
{
public:
	static const char * ConfigKey;

	bool AbilityFound() {
	//	Utils::MikuEcho('g', "Loaded Melody Sequence: ", GetKey());
		return true;
	}

	bool AbilityReady() {
		return true;
	}

	virtual bool CanCastSimultaneously() {
		return false;
	}

	void Cast() {
		char lCommand[MAX_STRING];
		EzCommand("/stopsong");
		sprintf_s(lCommand, "/melody %s", GetKey());
		EzCommand(lCommand);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuEcho(aSuccessType, aMessage, GetKey());
	}
};

const char * MelodyAbility::ConfigKey = "melody";
#endif