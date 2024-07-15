#pragma once
#include "Action.h"
#include "SpawnManager.h"
#include <vector>

class AbilityCollectionAbility : public Action
{
private:
	std::string _Key;
	bool _PermamentlyEnabled = false;
protected:
	SpawnType _SpawnType;
	std::vector<Action*> _Abilities;
	bool  _AllowEmpty = false;
	int _TargetID = -1;
	int _CastCounter = 0;
public:
	void AddAbility(Action* aAbility) {
		_Abilities.push_back(aAbility);
		_Ability = aAbility->GetAbility();
	}

	virtual ~AbilityCollectionAbility() {
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if ((*lIterator)->GetOwnedByManager()) continue;
			delete (*lIterator);
		}
		_Abilities.clear();
	}

	void DeleteAction(Action* aAction) {
		auto lCollection = std::find(_Abilities.begin(), _Abilities.end(), aAction);
		_Abilities.erase(lCollection);
		delete aAction;
	}

	void SetSpawnType(SpawnType aSpawnType) {
		_SpawnType = aSpawnType;
	}

	void SetTargetID(int aTargetID) {
		_TargetID = aTargetID;
	}

	void SetAllowEmpty(bool aAllowEmpty) {
		_AllowEmpty = aAllowEmpty;
	}

	void SetCastCounter(int aCounter) {
		_CastCounter = aCounter;
	}

	bool GetAllowEmpty() {
		return _AllowEmpty;
	}

	void DecrementCastCounter() {
		if (_CastCounter == 0) return;

		_CastCounter = _CastCounter - 1;
	}

	int GetCastCounter() {
		return _CastCounter;
	}

	PlayerClient * GetSpawn() {
		if (_TargetID == -1) {
			return SpawnManager::Get()->GetSpawn(_SpawnType);
		}
		else {
			return GetSpawnByID(_TargetID);
		}
	}

	virtual std::vector<Action*> GetAbilityCollection() {
		return _Abilities;
	}

	virtual bool HasAbilities() {
		return !_Abilities.empty();
	}

	bool AbilityFound() {
		return true;
	}

	bool AbilityReady() {
		return true;
	}

	virtual bool ShouldBeAbleToRecast() {
		return true;
	}

	virtual bool CanCastSimultaneously() {
		return false;
	}

	std::string GetKey() {
		return _Key;
	}

	void SetKey(std::string aKey) {
		_Key = aKey;
	}

	void ForcePermenentlyEnabled() {	
		_PermamentlyEnabled = true;
	}

	bool IsForcePermenentlyEnabled() {
		return _PermamentlyEnabled;
	}

	virtual bool IsEnabled() {
		return _PermamentlyEnabled || Action::IsEnabled();
	}

	virtual void Enable() {
		Action::Enable();
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			(*lIterator)->Enable();
		}
	}

	virtual void Reset() {
		Action::Reset();
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			(*lIterator)->Reset();
		}
	}

	virtual void Disable() {
		Reset();
		Action::Disable();
	}

	bool CanCastSelectedAbility(Action* aAbility) {
		return aAbility->IsEnabled() && !aAbility->IsCastSuccessful();
	}

	bool CastSelectedAbility(Action* aAbility) {
		if (aAbility->ExecuteAbility()) {
			DecrementCastCounter();
			return true;
		}
		return false;
	}

	bool CanCastSimultaneously(Action* aAbility) {
		return aAbility->CanCastSimultaneously();;
	}
};

