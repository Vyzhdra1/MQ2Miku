#pragma once
#include "CombatLocations.h"
#include "EncounterStep.h"

class Encounter
{
protected:
	const int ENCOUNTER_DELAY = 5000;
	int _StepNumber = 0;
	bool _Enabled = false;
	unsigned long _NextUpdate = 0;
	std::string _ValidZone = "";

	std::vector<EncounterStep*> _Steps;
	std::vector<EncounterStep*>::iterator _StepIterator;
public:
	Encounter() { 
		_Steps.clear();
	}

	~Encounter() {
		for (EncounterStep* lStep : _Steps) {
			delete lStep;
		}
	}

	void Enable() {
		_Enabled = true;
	}

	void Disable() {
		_Enabled = false;
	}

	bool IsEnabled() {
		return _Enabled;
	}

	bool UpdateClock() {
		_NextUpdate = Utils::GetClockTime();
	}

	void AddStep(EncounterStep * aStep) {
		_Steps.push_back(aStep);
	}

	void AddStep(std::function<bool()> aFunction, bool aIsMovementStep, std::string aValidZone) {
		AddStep(new EncounterStep(aFunction, aValidZone, aIsMovementStep));
	}

	void Next() {
		_StepIterator++;
		_StepNumber++;
	}

	virtual void Reset() {
		_StepIterator = _Steps.begin();
		_StepNumber = 1;
	}

	virtual std::string GetKey() = 0;

	virtual std::string GetName() = 0;

	void StepEcho(std::string aText) {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, aText + " | Step: ", std::to_string(_StepNumber) + " / " + std::to_string(_Steps.size()));
	}

	void IncrementDelay(int aDelay) {
		_NextUpdate = Utils::GetClockTime() + aDelay;
	}

	bool IsPauseElapsed() {
		unsigned long lClockTime = Utils::GetClockTime();
		return lClockTime > _NextUpdate;
	}

	bool IsProcessTime() {
		unsigned long lClockTime = Utils::GetClockTime();
		if (lClockTime < _NextUpdate) return false;

		_NextUpdate = lClockTime + ENCOUNTER_DELAY;

		return true;
	}

	bool ComeToMe(std::vector<std::string> aMembers) {
		bool lResult = false;

		for (std::string lName : aMembers) {
			PlayerClient* lMember = GetSpawnByName(lName.c_str());

			if (!lMember) return true;

			if (DistanceToSpawn(pLocalPC->pSpawn, lMember) > 175) {
				Utils::MikuEcho(Utils::WARNING_COLOR, "Come to me: ", DistanceToSpawn(pLocalPC->pSpawn, lMember));
				std::string lCommand = "/dex " + lName + " /easy " + pLocalPC->pSpawn->Name;
				EzCommand(lCommand.c_str());
				lResult = true;
			}
		}

		return lResult;
	}

	bool IsAllMembersInRange(std::vector<std::string> aMembers) {
		for (std::string lName : aMembers) {
			PlayerClient* lMember = GetSpawnByName(lName.c_str());

			if (!lMember) return false;

			if (DistanceToSpawn(pLocalPC->pSpawn, lMember) < 20) {
				return true;
			}
		}

		return true;
	}

	virtual void Run() = 0;

	virtual void SetStep(int aStep) = 0;
};