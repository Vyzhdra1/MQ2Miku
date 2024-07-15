#pragma once
#include <set>
#include <map>
#include <stack>
#include <string>
#include "Ability.h"
#include "AbilityCollectionAbility.h"
#include "Factory.h"
#include "Entry.h"
#include "Action.h"
#include "SpellBuffAction.h"
#include "TargetAction.h"

class DemandActionManager {
private:
	inline static DemandActionManager* _Manager = 0;
	std::map<AbilityCollectionAbility*, AbilityCollectionAbility*> _AbilitySets;

	bool GenerateCommand(Entry* aEntry) {
		AbilityCollectionAbility* lSet = ActionManager::Get()->GetAbilitySet(aEntry->GetDemandSet());

		if (!lSet) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Set not found Added: ", aEntry->GetDemandSet());
			return false;
		}

		Ability* lAbility = AbilityManager::Get()->GetAbility(aEntry->GetID());

		if (!lAbility) return false;

		AbilityCollectionAbility* lCollection = Factory::BuildAbilityCollection(aEntry);

		if (!lCollection) return false;

		Action* lAction = new Action();
		lAction->SetAbility(lAbility);
		lCollection->AddAbility(lAction);
		lSet->AddAbility(lCollection);

		_AbilitySets[lCollection] = lSet;

		return true;
	}
public:
	void ParseCommand(std::string aCommand) {
		Entry* lEntry = new Entry();
		bool lParseSuccessful = lEntry->ParseAsString(aCommand);

		if (lParseSuccessful) {
			if (!GenerateCommand(lEntry)) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to add Activity: ", aCommand);
			}
			else {
				Utils::MikuEcho(Utils::SUCCESS_COLOR, "Activity Added: ", aCommand);
			}
		}

		delete lEntry;
	}

	std::vector<AbilityCollectionAbility*> GetRemoveableAction() {
		std::vector<AbilityCollectionAbility*> lResult;
		for (const auto& [lKey, lValue] : _AbilitySets) {
			if (lKey->GetCastCounter() == 0 || lKey->IsPassedRemoveTime()) {
				lResult.push_back(lKey);
			}
		}
		return lResult;
	}

	void ReevaluateActions() {
		std::vector<AbilityCollectionAbility*> lRemoveableAction = GetRemoveableAction();

		for (AbilityCollectionAbility* lAction : lRemoveableAction) {
			AbilityCollectionAbility* lSet = _AbilitySets[lAction];
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Activity Removed: ", lAction->GetKey());
			_AbilitySets.erase(lAction);
			lSet->DeleteAction(lAction);
		}
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static DemandActionManager* Get() {
		if (!_Manager) {
			_Manager = new DemandActionManager();
		}
		return _Manager;
	}
};