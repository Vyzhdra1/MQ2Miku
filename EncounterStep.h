#pragma once
#include "Utils.h"

class EncounterStep
{
private:
	std::function<bool()> _Step = 0;
	bool _IsMovementStep = false;
	std::string _ValidZone = "";
public:
	EncounterStep(
		std::function<bool()> aFunction,
		std::string aValidZone,
		bool aIsMovementStep) {
		_Step = aFunction;
		_ValidZone = aValidZone;
		_IsMovementStep = aIsMovementStep;
	}

	bool IsMovementStep() {
		return _IsMovementStep;
	}

	bool IsValidZone() {
		return !_ValidZone.compare("") || Utils::IsZoneShortNameMatch(_ValidZone);
	}

	bool Execute() {
		return _Step();
	}
};
