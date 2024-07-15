#pragma once
#include <string>
#include <mq/Plugin.h>
#include "PlayerUtils.h"

class PlayerTarget {
private:
	std::set<long> _RequestedBuffs;
	std::string _Name;
	PlayerClient* _Character = 0;
	bool _IsTank;
	bool _IsGroupMember;
public:
	PlayerTarget() { }

	void LoadFromGroup(CGroupMember* aGroupMember) {
		if (aGroupMember == 0) return;

		_Character = aGroupMember->pSpawn;
		_IsTank = aGroupMember->IsMainTank();
		_IsGroupMember = true;
	}

	void LoadFromSpawn(PlayerClient* aSelf, bool aIsTank, bool aIsGroupMember) {

		_Character = GetSpawnByID(aSelf->SpawnID);
		_IsTank = aIsTank;
		_IsGroupMember = aIsGroupMember;
	}

	void LoadFromName(std::string aName, bool aIsTank) {
		_Name = aName;
		_Character = GetSpawnByName(aName.c_str());
		_IsTank = aIsTank;
		_IsGroupMember = false;
	}

	bool IsCharacterMatch(PlayerTarget* aCharacter) {
		return GetClient() == aCharacter->GetClient();
	}

	PlayerClient* GetClient() {
		return _Character;
	}

	void AddRequestedBuff(long aBuffId) {
		_RequestedBuffs.insert(aBuffId);
	}

	bool IsGroupMember() {
		return _IsGroupMember;
	}

	int GetHealthPerc() const {
		return PlayerUtils::ValuePerc(_Character->HPMax, _Character->HPCurrent);
	}

	bool IsLoaded() {
		return _Character != 0;
	}

	bool IsInRange(int aRange) {
		return DistanceToSpawn(pLocalPC->pSpawn, _Character) < aRange;
	}

	bool IsDead() {
		return _Character->StandState == STANDSTATE_DEAD;
	}

	bool IsMainTank() {
		return _IsTank;
	}

	std::string GetName() {
		return _Name;
	}

	bool operator < (PlayerTarget* aTarget) const
	{
		return (GetHealthPerc() < aTarget->GetHealthPerc());
	}
};