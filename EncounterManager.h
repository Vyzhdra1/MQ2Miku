#pragma once
#include <map>
#include <stack>
#include <string>
#include "Assessment.h"
#include "Encounter.h"
#include "RestlessAssaultGrindEncounter.h"

class EncounterManager {
private:
	const std::string STEP_STR = "step";
	const std::string LOAD_STR = "load";
	const std::string STOP_STR = "stop";
	const std::string START_STR = "start";
	const std::string PAUSE_STR = "pause";
	const std::string RESUME_STR = "resume";
	Encounter* _ActiveEncounter = 0;
public:
	EncounterManager() {}

	~EncounterManager() {
		if (_ActiveEncounter) {
			delete _ActiveEncounter;
			_ActiveEncounter = 0;
		}
	}

	std::string GetActiveEncounterKey() {
		if (_ActiveEncounter) {
			return _ActiveEncounter->GetKey();
		}
		return "";
	}

	bool IsEncounterActive() {
		return _ActiveEncounter;
	}

	std::vector<std::string> GetParamList(char * aInput) {
		std::vector<std::string> lResult;

		char* lParsePtr;
		char* lDelimiter = " ";

		char* lValue = strtok_s(aInput, lDelimiter, &lParsePtr);

		if (lValue) {
			lResult.push_back(lValue);
		}

		while (lValue) {
			lValue = strtok_s(NULL, lDelimiter, &lParsePtr);

			if (lValue) {
				lResult.push_back(lValue);
			}
		}

		return lResult;
	}

	void ParseCommand(std::string aCommand) {
		std::vector<std::string> lParams = GetParamList(const_cast<char*>(aCommand.c_str()));

		std::vector<std::string>::iterator lIterator = lParams.begin();

		if (lIterator == lParams.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "NO PARAMETER FOUND FOR MIKUENCOUNTER");
			return;
		}

		std::string lCommand = *lIterator;

		if (!STOP_STR.compare(lCommand)) {
			DeactivateEncounter();
			return;
		}

		if (!PAUSE_STR.compare(lCommand)) {
			PauseEncounter();
			return;
		}

		if (!RESUME_STR.compare(lCommand)) {
			ResumeEncounter();
			return;
		}

		if (!START_STR.compare(lCommand)) {
			StartEncounter();
			return;
		}

		lIterator++;

		if (lIterator == lParams.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No parameter for encounter: ", lCommand);
			return;
		}

		if (!STEP_STR.compare(lCommand)) {
			SetStep(*lIterator);
		}
		else if (!LOAD_STR.compare(lCommand)) {
			LoadEncounter(*lIterator);
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown Encounter Command");
		}
	}

	void SetStep(std::string aValue) {
		if (!IsEncounterActive()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "NO ENCOUNTER ACTIVE");
			return;
		}

		int lStepNumber = Utils::StrToInt(aValue.c_str(), -1);

		_ActiveEncounter->SetStep(lStepNumber);
	}

	void LoadEncounter(std::string aName) {
		if (!RestlessAssaultGrindEncounter::Key.compare(aName)) {
			DeleteEncounter();
			_ActiveEncounter = new RestlessAssaultGrindEncounter();
			_ActiveEncounter->Reset();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Encounter: ", _ActiveEncounter->GetName());
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Encounter: ", aName);
		}
	}

	void DeleteEncounter() {
		if (_ActiveEncounter) {
			delete _ActiveEncounter;
			_ActiveEncounter = 0;
		}
	}

	void DeactivateEncounter() {
		if (_ActiveEncounter) {
			DeleteEncounter();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Encounter Deactivated");
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No encounter active");
		}
	}

	void PauseEncounter() {
		if (_ActiveEncounter) {
			_ActiveEncounter->Disable();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Encounter Paused");
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No encounter active");
		}
	}

	void ResumeEncounter() {
		if (_ActiveEncounter) {
			_ActiveEncounter->Enable();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Encounter Resumed");
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No encounter active");
		}
	}

	void StartEncounter() {
		if (_ActiveEncounter) {
			_ActiveEncounter->Enable();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Starting Encounter: ", _ActiveEncounter->GetName());
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "No encounter active");
		}
	}

	void ProcessEncounter() {
		if (_ActiveEncounter) {
			_ActiveEncounter->Run();
		}
	}
};
