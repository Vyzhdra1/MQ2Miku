#pragma once
#include <mq/Plugin.h>
#include "PlayerUtils.h"
#include "PlayerTarget.h"
#include "SettingManager.h"
#include "StringListQuery.h"

enum HealStrategy {GROUPTANK, RAIDTANK};
class HealingManager
{
private:
	inline static HealingManager* _Manager = 0;

	std::vector<std::string> _TankNames;

	std::vector<PlayerTarget*> _Targets;

	int _AverageGroupHealth;
	PlayerClient* _DeadPlayer;
	PlayerClient* _Tank;
	PlayerClient* _Lowest;

	bool IsLoaded(PlayerTarget* aTarget) {
		for (PlayerTarget* lTarget : _Targets) {
			if (lTarget->IsCharacterMatch(aTarget)) {
				return true;
			}
		}
		return false;
	}

	void AddTarget(PlayerTarget* aTarget) {
		if (!aTarget->IsLoaded() || IsLoaded(aTarget)) {
			delete aTarget;
			return;
		}

		_Targets.push_back(aTarget);
	}

	void LoadManualTarget(std::string aName) {
		PlayerTarget* lTarget = new PlayerTarget();
		lTarget->LoadFromName(aName, true);

		if (!lTarget->IsLoaded() || IsLoaded(lTarget))
		{
			delete lTarget;
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Failed to watch: ", lTarget->GetName());
			return;
		}
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Registered watch: ", lTarget->GetName());
		_Targets.push_back(lTarget);
	}

	void LoadXTargets() {
		ExtendedTargetList* lTargetList = GetCharInfo()->pExtendedTargetList;

		if (!lTargetList) return;

		int lCount = 0;

		for (ExtendedTargetSlot* lIterator = lTargetList->begin(); lIterator != lTargetList->end(); lIterator++) {
			if (!lIterator) continue;

			if (!lIterator->SpawnID) continue;

			PlayerClient* lSpawn = GetSpawnByID(lIterator->SpawnID);

			if (!lSpawn) continue;

			if (lSpawn->Type != SPAWN_PLAYER) continue;

			PlayerTarget* lTarget = new PlayerTarget();
			lTarget->LoadFromSpawn(lSpawn, true, false);
			AddTarget(lTarget);
		}
	}

	void LoadTargets() {
		DestroyTargets();

		if (pLocalPC->Group) {
			for (CGroupMember**  lIterator = pLocalPC->Group->begin(); lIterator != pLocalPC->Group->end(); lIterator++)
			{
				PlayerTarget* lTarget = new PlayerTarget();
				lTarget->LoadFromGroup(*lIterator);
				AddTarget(lTarget);
			}
		}
		else {
			PlayerTarget* lTarget = new PlayerTarget();
			lTarget->LoadFromSpawn(pLocalPlayer, false, true);
			AddTarget(lTarget);
		}
	}

	void DestroyTargets() {
		for (PlayerTarget* lTarget : _Targets) {
			delete lTarget;
		}
		_Targets.clear();
	}

	void AssessCorpses() {
		_DeadPlayer = 0;

		if (PlayerUtils::InCombat()) return;

		if (!PlayerUtils::IsRezer()) return;

		MQSpawnSearch ssSpawn;
		ClearSearchSpawn(&ssSpawn);

		ssSpawn.FRadius = SettingManager::Get()->GetHealRange();
		ssSpawn.SpawnType = PCCORPSE;
		ssSpawn.GuildID = pLocalPC->GuildID;

		PSPAWNINFO lCorpse = NthNearestSpawn(&ssSpawn, 1, pControlledPlayer);

		if (lCorpse) {
			_DeadPlayer = lCorpse;
		}
	}

	int GetAverageGroupHp() {
		int lTotalHealth = 0;
		int lGroupMembers = 0;
		for (PlayerTarget* lTarget : _Targets) {
			if (!lTarget->IsGroupMember() || !lTarget->GetHealthPerc() || lTarget->IsDead()) continue;

			lTotalHealth += lTarget->GetHealthPerc();
			lGroupMembers++;
		}

		if (!lTotalHealth || (lGroupMembers < 3)) return 100;

		return lTotalHealth / lGroupMembers;
	}

public:
	PlayerClient* GetSpawn(SpawnType aSpawnType) {
		switch (aSpawnType) {
		case TANK: return _Tank;
		case LOWEST_HP_GROUP: return _Lowest;
		case DEAD_PLAYER: return _DeadPlayer;
		}

		return 0;
	}

	PlayerClient* GetTank() {
		for (PlayerTarget* lTarget : _Targets) {
			if (lTarget->IsDead()) continue;
			if (lTarget->IsGroupMember() && lTarget->IsMainTank()) {
				return lTarget->GetClient();
			}
		}
		return 0;
	}

	PlayerClient* GetLowestHP() {
		PlayerTarget* lLowestHP = 0;
		for (PlayerTarget* lTarget : _Targets) {
			if (lTarget->IsDead()) continue;
			if (!lLowestHP || (lLowestHP->GetHealthPerc() > lTarget->GetHealthPerc())) lLowestHP = lTarget;
		}

		if (lLowestHP) return lLowestHP->GetClient();
		return 0;
	}

	int GetRealGroupMemberCount() {
		if (!pLocalPC->Group) {
			return 0;
		}
		return pLocalPC->Group->GetNumberOfMembers();
	}

	int GetAverageGroupHealth() {
		return _AverageGroupHealth;
	}

	int FindAvailableXTargetSlot() {
		ExtendedTargetList* lTargetList = GetCharInfo()->pExtendedTargetList;

		if (!lTargetList) return -1;

		int lIndex = 0;
		for (ExtendedTargetSlot* lIterator = lTargetList->begin(); lIterator != lTargetList->end(); lIterator++) {
			if (!lIterator) return lIndex;

			if (!lIterator->SpawnID) return lIndex;

			PlayerClient* lSpawn = GetSpawnByID(lIterator->SpawnID);

			if (!lSpawn) return lIndex;

			if (lSpawn->Type != SPAWN_PLAYER) return lIndex;

			lIndex++;
		}
		return -1;
	}

	void RegisterTarget(std::string aName) {
		int lAvailableSlot = FindAvailableXTargetSlot();
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Registered watch: ", aName);
		Utils::MikuSendCommand("/xtarget set " + std::to_string(lAvailableSlot) + " " + aName);
	}

	void HealingManager::Assess() {
		LoadTargets();
		AssessCorpses();

		_AverageGroupHealth = GetAverageGroupHp();
		_Tank = GetTank();
		_Lowest = GetLowestHP();
	}

	static void Deinit() {
		if (_Manager) {
			_Manager->DestroyTargets();
			delete _Manager;
			_Manager = 0;
		}
	}

	static HealingManager* Get() {
		if (!_Manager) {
			_Manager = new HealingManager();
		}
		return _Manager;
	}
};