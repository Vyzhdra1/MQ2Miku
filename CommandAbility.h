#pragma once
#include "Ability.h"
#include "PlayerUtils.h"
#include "DbCommand.h"

class CommandAbility : public Ability
{
private:
public:
	static const char* ConfigKey;

	CommandAbility() {}

	CommandAbility(DbCommand* aDbCommand) {
		SetKey(aDbCommand->GetCommandText());
		SetName(aDbCommand->GetCommandKey());
	}

	bool AbilityFound() {
		return true;
	}

	virtual bool IsLoaded() override {
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
		EzCommand(GetKey().c_str());
	}

	void EchoCastMessage(char aSuccessType, char* aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, GetName());
	}

	virtual void EchoLoadSuccessMessage() {
		std::string lMessage = "Key: " + GetName() + " | Command: " + GetKey();
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Command: ", lMessage);
	}

	virtual std::string GetType() {
		return CommandAbility::ConfigKey;
	}
};

const char* CommandAbility::ConfigKey = "command";
