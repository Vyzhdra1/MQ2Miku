#ifndef H_CommandAbility
#define H_CommandAbility
#include "Ability.h"
#include "PlayerUtils.h"

class CommandAbility : public Ability
{
private:
public:
	static const char* ConfigKey;

	bool AbilityFound() {
		return true;
	}

	bool AbilityReady() {
		return
			PlayerUtils::CanExecuteAbility() &&
			!Utils::IsCasting();
	}

	virtual bool CanCastSimultaneously() {
		return true;
	}

	void Cast() {
		EzCommand(GetKey());
	}

	void EchoCastMessage(char aSuccessType, char* aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, GetName());
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Command: ", GetKey());
	}

	virtual std::string GetType() {
		return CommandAbility::ConfigKey;
	}
};

const char* CommandAbility::ConfigKey = "command";
#endif#pragma once
