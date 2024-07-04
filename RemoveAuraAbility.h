#pragma once
#ifndef H_RemoveAuraAbility
#define H_RemoveAuraAbility
#include "Ability.h"
#include "PlayerUtils.h"

class RemoveAuraAbility : public Ability
{
private:
public:
	static const char* ConfigKey;


	virtual bool IsLoaded() override {
		return true;
	}

	bool AbilityFound() {
		return true;
	}

	bool AbilityReady() {
		return true;
	}

	virtual bool CanCastSimultaneously() {
		return true;
	}

	void Cast() {
		EzCommand("/notify AuraWindow AuraList listselect 1");
		EzCommand("/notify AuraWindow 0 contextmenu 1");
	}

	void EchoCastMessage(char aSuccessType, char* aMessage) {
		Utils::MikuEcho(aSuccessType, aMessage, "Remove Aura");
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded: ", "Remove Aura");
	}
};

const char* RemoveAuraAbility::ConfigKey = "removeaura";
#endif