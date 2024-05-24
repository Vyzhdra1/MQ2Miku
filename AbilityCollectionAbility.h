#ifndef H_AbilityCollectionAbility
#define H_AbilityCollectionAbility
#include "Action.h"
#include <vector>

class AbilityCollectionAbility : public Action
{
private:
	std::string _Key;
	bool _PermamentlyEnabled = false;
protected:
	SpawnType _SpawnType;
	std::vector<Action*> _Abilities;
public:
	static const char * PASSIVE_CLASS;
	static const char * COMBAT_CLASS;
	static const char * BUFF_CLASS;

	void AddAbility(Action* aAbility) {
		_Abilities.push_back(aAbility);
	}

	virtual ~AbilityCollectionAbility() {
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			delete (*lIterator);
		}
		_Abilities.clear();
	}

	void SetSpawnType(SpawnType aSpawnType) {
		_SpawnType = aSpawnType;
	}

	virtual std::vector<Action*> GetAbilityCollection() {
		return _Abilities;
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
		return aAbility->ExecuteAbility();
	}

	bool CanCastSimultaneously(Action* aAbility) {
		return aAbility->CanCastSimultaneously();;
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {};
};

const char * AbilityCollectionAbility::PASSIVE_CLASS = "passive";
const char * AbilityCollectionAbility::COMBAT_CLASS = "combat";
const char * AbilityCollectionAbility::BUFF_CLASS = "buff";

#endif