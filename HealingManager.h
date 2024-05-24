#pragma once
#include <mq/Plugin.h>
#include "PlayerUtils.h"
#include "PlayerTarget.h"
#include "SettingManager.h"

class HealingManager
{
private:
	inline static HealingManager* _Manager = 0;

	std::vector<PlayerTarget*> _Targets;
	std::vector<PlayerTarget*> _OrderedHealth;
	std::vector<PlayerTarget*> _GroupMembers;
	std::vector<PlayerTarget*> _Tanks;

	int _AverageGroupHealth;
	PlayerClient* _TankTarget;
	PlayerClient* _DeadPlayer;

	bool IsLoaded(PlayerTarget* aTarget) {
		for (PlayerTarget* lTarget : _Targets) {
			if (lTarget->IsCharacterMatch(aTarget)) {
				return true;
			}
		}
		return false;
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

	void LoadTargets() {
		DestroyTargets();

		if (pLocalPC->Group) {
			for (CGroupMember**  lIterator = pLocalPC->Group->begin(); lIterator != pLocalPC->Group->end(); lIterator++)
			{
				PlayerTarget* lTarget = new PlayerTarget();
				lTarget->LoadFromGroup(*lIterator);

				if (!lTarget->IsLoaded())
				{
					delete lTarget;
					continue;
				}

				_Targets.push_back(lTarget);
			}
		}
	}

	void DestroyTargets() {
		for (PlayerTarget* lTarget : _Targets) {
			delete lTarget;
		}
		_Targets.clear();
	}

	void AssessCorpses() {
		if (PlayerUtils::InCombat()) return;

		if (!PlayerUtils::IsRezer()) return;

		if (!_DeadPlayer) return;

		MQSpawnSearch ssSpawn;
		ClearSearchSpawn(&ssSpawn);
		ssSpawn.FRadius = 100;

		for (PlayerTarget* lTarget : _Targets) {
			MQSpawnSearch ssSpawn;
			ClearSearchSpawn(&ssSpawn);

			ssSpawn.FRadius = 100;
			ssSpawn.SpawnType = PCCORPSE;
			strcpy_s(ssSpawn.szName, lTarget->GetClient()->Name);

			PSPAWNINFO lCorpse = NthNearestSpawn(&ssSpawn, 1, pControlledPlayer);

			if (lCorpse) {
				_DeadPlayer = lCorpse;
				return;
			}
		}
	}

	int GetAverageGroupHp() {
		int lTotalHealth = 0;
		int lGroupMembers = 0;
		for (PlayerTarget* lTarget : _Targets) {
			if (!lTarget->IsGroupMember()) continue;

			lTotalHealth += lTarget->GetHealthPerc();
			lGroupMembers++;
		}

		if (!lTotalHealth || lGroupMembers) return 100;

		return lTotalHealth / lGroupMembers;
	}

	void ResetProperties() {
		_OrderedHealth.clear();
		_Tanks.clear();

		_TankTarget = 0;
		_DeadPlayer = 0;
	}

	void HealingManager::AssessHealth() {
		int lTotalGroupHealth = 0;

		for (PlayerTarget* lTarget : _Targets) {
			if (!lTarget->IsInRange(SettingManager::Get()->GetHealRange())) continue;

			if (lTarget->IsDead()) {
				if (lTarget->IsInRange(SettingManager::Get()->GetRezRange())) {
					_DeadPlayer = lTarget->GetClient();
				}
				continue;
			}

			_OrderedHealth.push_back(lTarget);

			if (lTarget->IsMainTank()) {
				if (!GetGroupTank()) {
					int lTankTargetID = lTarget->GetClient()->TargetOfTarget;
					if (lTankTargetID > 0) {
						_TankTarget = GetSpawnByID(lTankTargetID);
					}
				}
				if (lTarget->IsGroupMember()) {
					_Tanks.insert(_Tanks.begin(), lTarget);
				}
				else {
					_Tanks.push_back(lTarget);
				}

				continue;
			}

			if (!lTarget->IsGroupMember())
			{
				_GroupMembers.push_back(lTarget);
			}
		}
		
		std::sort(_OrderedHealth.begin(), _OrderedHealth.end());
	}

public:
	PlayerClient* GetSpawn(SpawnType aSpawnType) {
		switch (aSpawnType) {
		case TANK: return GetTank();
		case TARGET_OF_TANK: return _TankTarget;
		case LOWEST_HP_GROUP: return GetLowestHP();
		case DEAD_PLAYER: return _DeadPlayer;
		}

		return 0;
	}

	PlayerClient* GetTank() {
		if (_Tanks.empty()) return 0;
			
		return _Tanks.front()->GetClient();
	}

	PlayerTarget* GetGroupTank() {
		for (PlayerTarget* lTank : _Tanks) {
			if (lTank->IsGroupMember()) return lTank;
		}
		return 0;
	}

	PlayerClient* GetLowestHP() {
		if (_OrderedHealth.empty()) return 0;

		return _OrderedHealth.front()->GetClient();
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

	void HealingManager::Assess() {
		ResetProperties();

		LoadTargets();
		AssessHealth();
		AssessCorpses();

		_AverageGroupHealth = GetAverageGroupHp();
	}

	void RegisterTarget(std::string aName) {
		LoadManualTarget(aName);
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