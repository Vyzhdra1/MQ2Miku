#pragma once
#include <set>
#include <map>
#include <stack>
#include <string>
#include "Ability.h"
#include "AbilityCollectionAbility.h"

class ActionManager {
private:
	std::map<std::string, AbilityCollectionAbility*> _AbilitySets;
	std::vector<AbilityCollectionAbility*> _ActiveAbilities;

	void RemoveFromActiveCollection(std::vector<AbilityCollectionAbility*> aRemoveableAbilities) {
		for (std::vector<AbilityCollectionAbility*>::iterator lIterator = aRemoveableAbilities.begin(); lIterator != aRemoveableAbilities.end(); lIterator++) {
			auto lCollection = std::find(_ActiveAbilities.begin(), _ActiveAbilities.end(), (*lIterator));
			_ActiveAbilities.erase(lCollection);
		}
	} 
public:
	void ActivatePermanentlyEnabledSets() {
		for (std::map<std::string, AbilityCollectionAbility*>::iterator lIterator = _AbilitySets.begin(); lIterator != _AbilitySets.end(); lIterator++) {
			if (lIterator->second->IsForcePermenentlyEnabled()) {
				std::string lKey = lIterator->second->GetKey();
				ActivateAbility(lKey);
			}
		}
	}

	~ActionManager() {
		ClearActiveAbilities();
		std::map<std::string, AbilityCollectionAbility*>::iterator lIterator = _AbilitySets.begin();
		while (lIterator != _AbilitySets.end()) {
			delete lIterator->second;
			lIterator++;
		}

		_AbilitySets.clear();
	}

	void ActivateAbility(std::string aName) {
		for (std::vector<AbilityCollectionAbility*>::iterator lIterator = _ActiveAbilities.begin(); lIterator != _ActiveAbilities.end(); lIterator++) {
			if (!aName.compare((*lIterator)->GetKey())) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Already Activated: ", aName);
				return;
			}
		}

		AbilityCollectionAbility* lCollection = GetAbilitySet(aName);

		if (lCollection) {
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Activated: ", aName);
			lCollection->Enable();
			_ActiveAbilities.push_back(lCollection);
		}
	}

	void ClearActiveAbilities() {
		if (_ActiveAbilities.size() == 0) {
			return;
		}

		std::vector<AbilityCollectionAbility*> lAbilitiesToDeactivate;

		for (std::vector<AbilityCollectionAbility*>::iterator lIterator = _ActiveAbilities.begin(); lIterator != _ActiveAbilities.end(); lIterator++) {
			if (!(*lIterator)->IsForcePermenentlyEnabled()) {
				(*lIterator)->Disable();
				lAbilitiesToDeactivate.push_back((*lIterator));
			}
		}

		RemoveFromActiveCollection(lAbilitiesToDeactivate);
	}

	void ExecuteAbilities() {
		if (_ActiveAbilities.size() == 0) {
			return;
		}

		std::vector<AbilityCollectionAbility*> lAbilitiesToDeactivate;

		for (std::vector<AbilityCollectionAbility*>::iterator lIterator = _ActiveAbilities.begin(); lIterator != _ActiveAbilities.end(); lIterator++) {
			(*lIterator)->ExecuteAbility();
			if (!(*lIterator)->IsEnabled()) {
				lAbilitiesToDeactivate.push_back((*lIterator));
			}
		}

		RemoveFromActiveCollection(lAbilitiesToDeactivate);
	}

	void AddAbilitySet(std::string aKey, AbilityCollectionAbility* aAbilitySet) {
		_AbilitySets[aKey] = aAbilitySet;
		aAbilitySet->Disable();
	}

	std::map<std::string, AbilityCollectionAbility*>& GetAbilitySets() {
		return _AbilitySets;
	}

	AbilityCollectionAbility* GetAbilitySet(std::string aKey) {
		if (_AbilitySets.find(aKey) == _AbilitySets.end()) return 0;
		return _AbilitySets[aKey];
	}
};