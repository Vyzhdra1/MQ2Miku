#pragma once
#include <string>
#include <mq/Plugin.h>
#include "PlayerUtils.h"

class PlayerBuffProperties {
private:
	std::string _CasterClass;
	std::string _Key;
	std::string _ID;
public:
	PlayerBuffProperties(
		std::string aCasterClass,
		std::string aKey,
		std::string aID
	) {
		_CasterClass = aCasterClass;
		_Key = aKey;
		_ID = aID;	
	}

	bool HasBuff() {

	}

	void LoadFromGroup(CGroupMember* aGroupMember) {
		if (aGroupMember == 0) return;

		_Character = aGroupMember->pSpawn;
		_IsTank = aGroupMember->IsMainTank();
		_IsGroupMember = true;
	}

	void LoadFromName(std::string aName, bool aIsTank) {
		_Character = GetSpawnByName(aName.c_str());
		_IsTank = aIsTank;
		_IsGroupMember = false;
	}

	bool IsCharacterMatch(PlayerBuffProperties* aCharacter) {
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

	bool operator < (PlayerTarget* aTarget) const
	{
		return (GetHealthPerc() < aTarget->GetHealthPerc());
	}
};