#pragma once
#include "Encounter.h"
#include "Utils.h"
#include "PlayerUtils.h"
#include "Location.h"
#include "GameManager.h"

class RestlessAssaultGrindEncounter : public Encounter
{
private:
	const std::string BASE_ZONE = "greatdividetwo";
	const std::string MISSION = "greatdividetwo_mission";
	const int LEVEL_LIMIT = 115;

	bool _PullAllowed = false;

	CombatLocationManager* _CombatLocationManager = 0;
	CombatLocationManager* _LeveleeLocationManager = 0;

	std::vector<std::string> _Helpers;
	std::vector<std::string> _LowLevels;
	std::vector<std::string> _Group;

	void InitCombatLocations(
		CombatLocationManager** aLocationManager, 
		int aDelay) {
		if (*aLocationManager) {
			delete* aLocationManager;
		}

		CombatLocationManager* lLocationManager = new CombatLocationManager();

		//Buff
		for (int i = 0; i < aDelay + 1; i++) {
			lLocationManager->AddCombatLocation(-1658, -305, -256);
		}

		//lLocationManager->AddCombatLocation(-1669, -126, -264);
		lLocationManager->AddCombatLocation(-1739, -101, -264);


		lLocationManager->AddCombatLocation(-1802, -171, -274);
		lLocationManager->AddCombatLocation(-1937, -179, -297);
		lLocationManager->AddCombatLocation(-2185, -170, -329);
		lLocationManager->AddCombatLocation(-2328, -138, -348);
		lLocationManager->AddCombatLocation(-2440, -132, -363);
		lLocationManager->AddCombatLocation(-2708, -151, -402);
		lLocationManager->AddCombatLocation(-2995, -178, -438);
		lLocationManager->AddCombatLocation(-3121, -188, -450);
		lLocationManager->AddCombatLocation(-2975, -242, -428);		
		lLocationManager->AddCombatLocation(-2874, -330, -420);

		lLocationManager->AddCombatLocation(-2920, -483, -442);
		lLocationManager->AddCombatLocation(-2867, -548, -449);
		lLocationManager->AddCombatLocation(-2846, -376, -417);
		lLocationManager->AddCombatLocation(-2731, -407, -400);
		lLocationManager->AddCombatLocation(-2708, -453, -394);
		lLocationManager->AddCombatLocation(-2596, -544, -373);
		lLocationManager->AddCombatLocation(-2546, -707, -373);
		lLocationManager->AddCombatLocation(-2611, -434, -385);
		lLocationManager->AddCombatLocation(-2300, -502, -343);

		lLocationManager->AddCombatLocation(-2344, -601, -349);
		lLocationManager->AddCombatLocation(-2216, -715, -327);

		lLocationManager->AddCombatLocation(-2241, -542, -337);
		lLocationManager->AddCombatLocation(-2176, -593, -306);

		lLocationManager->AddCombatLocation(-2019, -542, -303);
		lLocationManager->AddCombatLocation(-2070, -701, -303);
		lLocationManager->AddCombatLocation(-1955, -722, -317);
		lLocationManager->AddCombatLocation(-2000, -332, -304);

		lLocationManager->SetCommand("/dex ");
		lLocationManager->Reset();
		*aLocationManager = lLocationManager;
	}

	void Move() {
		_CombatLocationManager->Move(_Helpers);
		_LeveleeLocationManager->Move(_LowLevels);
	}

	void InitGroup() {
		_Helpers.clear();
		_LowLevels.clear();
		_Group.clear();
		bool lResult = true;

		CGroupMember** lIterator;

		if (!pLocalPC->Group) {
			return;
		}

		for (lIterator = pLocalPC->Group->begin(); lIterator != pLocalPC->Group->end(); lIterator++)
		{
			if (!(*lIterator)) {
				continue;
			}

			PlayerClient* lGroupMember = (*lIterator)->pSpawn;

			if (!lGroupMember) return;

			if (lGroupMember->Level < LEVEL_LIMIT) {
				_LowLevels.push_back(lGroupMember->Name);
			}
			else {
				_Helpers.push_back(lGroupMember->Name);
			}
			_Group.push_back(lGroupMember->Name);
		}
	}

	//________________________________________________________________________________________________
	// 	   
	// Pre Instance
	// 
	//________________________________________________________________________________________________

	bool MoveToQuestGiver() {
		StepEcho("Move to Zrelik the Brave");

		EzCommand("/dgae group /easy Zrelik the Brave");
		EzCommand("/dgae group /target Zrelik");

		return true;
	}

	bool GetQuest() {
		StepEcho("Obtain Quest");

		EzCommand("/dgae group /target Zrelik");
		EzCommand("/say small");

		IncrementDelay(70000);

		return true;
	}

	bool WaitToZoneIn() {
		StepEcho("Enter Zone");

		int _WaitTime = 20;

		for (std::string lName : _Group) {
			std::string lCommand = lName.compare(GetCharInfo()->Name) ? "/dex " + lName + " " : "";
			lCommand += " /multiline ; /target zrelik ; /timed " + std::to_string(_WaitTime) + " /say Ready ; ";
			EzCommand(lCommand.c_str());
			_WaitTime += 20;
		}

		return true;
	}

	//________________________________________________________________________________________________
	// 	   
	// Pre Fight
	// 
	//________________________________________________________________________________________________

	bool MoveToBuffPosition() {
		StepEcho("Move to Buff Location");

		return true;
	}

	bool CommandBuffs() {
		StepEcho("Buffing");
		EzCommand("/mikubc c=act;b=dgae group;a=Buff");

		IncrementDelay(70000);

		return true;
	}

	bool LessonBurn() {
		StepEcho("Lesson Burn");
		EzCommand("/dgae group /alt act 481");

		IncrementDelay(10000);

		return true;
	}

	//________________________________________________________________________________________________
	// 	   
	// Fight
	// 
	//________________________________________________________________________________________________

	bool MoveToCombat() {
		StepEcho("Moving to Combat Area");

		_PullAllowed = true;

		return true;
	}

	bool KickParty() {
		StepEcho("Kicking Party");
		EzCommand("/kickp task");

		return true;
	}

	bool AcceptKickParty() {
		StepEcho("Accepting Kick");
		EzCommand("/yes");

		IncrementDelay(70000);

		return true;
	}

	bool Restart() {
		StepEcho("Restarting");

		Reset();
		Enable();

		return false;
	}
public:
	static const std::string Key;

	virtual std::string GetKey() {
		return Key;
	}

	virtual std::string GetName() {
		return "Restless Assault (Grind)";
	}

	void Combat() {
		int lSpawnID = PlayerUtils::GetClosestXTarget();

		if (!lSpawnID) return;

		if (pTarget) {
			EzCommand("/face");
		}

		if (!pTarget || (pTarget->SpawnID != lSpawnID)) {
			Utils::TargetByID(GetCharInfo()->pSpawn, lSpawnID);
		}

		if (!PlayerUtils::IsAutoOn() || IsPauseElapsed()) {
			EzCommand("/easy stop");
			EzCommand("/mikubc c=atk;b=dgae group");
		}

	}

	int GetClosestTarget() {
		MQSpawnSearch lSpawnSearch;
		ClearSearchSpawn(&lSpawnSearch);
		lSpawnSearch.FRadius = 200.0f;
		lSpawnSearch.bLoS = true;
		lSpawnSearch.SpawnType = NPC;

		SPAWNINFO* lSpawn = NthNearestSpawn(&lSpawnSearch, 1, pControlledPlayer);

		if (!lSpawn) return 0;
		
		return lSpawn->SpawnID;
	}

	bool Pull() {
		if (!Utils::IsZoneShortNameMatch(MISSION)) return false;

		if (!_PullAllowed) return false;

		int lSpawnID = PlayerUtils::GetClosestXTarget();

		if (lSpawnID) return false;

		lSpawnID = GetClosestTarget();

		if (!lSpawnID) return false;

		Utils::TargetByID(GetCharInfo()->pSpawn, lSpawnID);
		EzCommand("/stand");
		EzCommand("/face");
		EzCommand("/mikuact a=Pull");

		return true;
	}

	~RestlessAssaultGrindEncounter() {
		if (_CombatLocationManager) delete _CombatLocationManager;
		if (_LeveleeLocationManager) delete _LeveleeLocationManager;

		Encounter::~Encounter();
	}

	void AddSteps() {
		if (_Steps.size() != 0) return;

		AddStep([this]() { return MoveToQuestGiver(); }, false, BASE_ZONE);
		AddStep([this]() { return GetQuest(); }, false, BASE_ZONE);
		AddStep([this]() { return WaitToZoneIn(); }, false, BASE_ZONE);

		AddStep([this]() { return MoveToBuffPosition(); }, true, MISSION);
		AddStep([this]() { return CommandBuffs(); }, false, MISSION);
		AddStep([this]() { return LessonBurn(); }, false, MISSION);

		for (int i = 0; i < _CombatLocationManager->GetLocationCount() - 1; i++) {
			AddStep([this]() { return MoveToCombat(); }, true, MISSION);
		}

		AddStep([this]() { return KickParty(); }, false, MISSION);
		AddStep([this]() { return AcceptKickParty(); }, false, MISSION);
		AddStep([this]() { return Restart(); }, false, BASE_ZONE);
	}

	virtual void Reset() {
		InitGroup();
		InitCombatLocations(&_CombatLocationManager, 0);
		InitCombatLocations(&_LeveleeLocationManager, 1);

		AddSteps();

		Encounter::Reset();

		_PullAllowed = false;

		Disable();
	}

	void SetStep(int aStep) {
		Reset();

		if (aStep <= 1) {
			return;
		}

		while (_StepIterator != _Steps.end()) {			
			if ((*_StepIterator)->IsMovementStep()) {
				_CombatLocationManager->Next();
				_LeveleeLocationManager->Next();
			}

			_StepIterator++;
			_StepNumber++;

			if (_StepNumber >= aStep) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Step reset to: ", aStep);
				return;
			}
		}
	}

	void Run() {

		if (!PlayerUtils::CanExecuteAbility()) {
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Cannot execute ability");
			return;
		}

		Combat();

		if (!_Enabled) {
			return;
		}

		if (!IsProcessTime()) return;

		if (!(*_StepIterator)) {
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Run completed!");
			Disable();
			return;
		}

		if (GameManager::GetAssessmentManager()->GetGroupAssessment()->GetRealGroupMemberCount() != 6) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "MISSING GROUP MEMBERS!");
			return;
		}

		if (!(*_StepIterator)->IsValidZone()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "CURRENT ZONE IS INVALID");
			return;
		}

		if (!MikuMovementUtils::IsMoving() && ComeToMe(_Helpers)) return;

		if (GetCharInfo()->pExtendedTargetList->GetSlot(0) && GetCharInfo()->pExtendedTargetList->GetSlot(0)->SpawnID) return;

		if (MikuMovementUtils::IsMoving()) return;

		if (!IsAllMembersInRange(_Helpers)) {
			Utils::MikuEcho(Utils::WARNING_COLOR, "Waiting for Group");
			return;
		}

		if (Pull()) return;

		if (!(*_StepIterator)->Execute()) {
			return;
		}
		
		if ((*_StepIterator)->IsMovementStep()) {
			Move();
		}

		Next();
	}
};
const std::string RestlessAssaultGrindEncounter::Key = "restless";