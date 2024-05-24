#pragma once
#include "Utils.h"

class CombatLocation {
private:
	int _X;
	int _Y;
	int _Z;
public:
	CombatLocation(int aY, int aX, int aZ) {
		_Y = aY;
		_X = aX;
		_Z = aZ;
	}

	void Move(std::string aCommand) {
		std::string lCommand = aCommand + "/nav locyxz " + std::to_string(_Y) + ", " + std::to_string(_X) + ", " + std::to_string(_Z);
		EzCommand(lCommand.c_str());
	}
};

class CombatLocationManager
{
private:
	std::vector<CombatLocation*> _CombatLocations;
	std::vector<CombatLocation*>::iterator _CombatLocationIterator;
	std::string _Command = "";
public:
	CombatLocationManager() {
		_CombatLocations.clear();
	}

	~CombatLocationManager() {
		for (std::vector<CombatLocation*>::iterator lIterator = _CombatLocations.begin(); lIterator != _CombatLocations.end(); lIterator++) {
			delete (*lIterator);
		}
		_CombatLocations.clear();
	}

	void AddCombatLocation(int aX, int aY, int aZ) {
		_CombatLocations.push_back(new CombatLocation(aX, aY, aZ));
	}

	void SetCommand(std::string aCommand) {
		_Command = aCommand;
	}

	int GetLocationCount() {
		return (int) _CombatLocations.size();
	}

	void Reset() {
		_CombatLocationIterator = _CombatLocations.begin();
	}

	void Move(std::vector<std::string> aNames) {
		if (!(*_CombatLocationIterator) || (_CombatLocationIterator == _CombatLocations.end())) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "NO LOCATION FOUND!");
			return;
		}

		for (std::string lName : aNames) {
			std::string lCommand = lName.compare(GetCharInfo()->Name) ? _Command + lName + " " : "";
			(*_CombatLocationIterator)->Move(lCommand);
		}
		_CombatLocationIterator++;
	}

	void Move(std::string aCommand) {
		if (!(*_CombatLocationIterator)) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "NO LOCATION FOUND!");
			return;
		}

		(*_CombatLocationIterator)->Move(aCommand);
		_CombatLocationIterator++;
	}

	void Next() {
		_CombatLocationIterator++;
	}
};
