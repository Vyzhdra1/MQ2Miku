#pragma once
#include "Ability.h"
#include "ItemAbility.h"
#include "BooleanCondition.h"

class ItemMissingCondition : public BooleanCondition
{	
private:
	ItemAbility* _ItemAbility = 0;
public:
	static const char * Key;

	ItemMissingCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	~ItemMissingCondition() {
		if (!_ItemAbility) return;

		//delete _ItemAbility;
	}

	bool ConditionMet(Ability * aAbility) {
		if (!_ItemAbility) return false;

		return !_ItemAbility->AbilityFound() == _BooleanCondition;
	}

	virtual void ParseNextValue(std::string aValue) override {
		if (_ItemAbility) delete _ItemAbility;

		Ability* lAbility = AbilityManager::Get()->GetAbility(aValue);

		if (!lAbility) return;
		if (!dynamic_cast<ItemAbility*>(lAbility)) return;

		_ItemAbility = (ItemAbility*)lAbility;

		//_ItemAbility = new ItemAbility();
		//_ItemAbility->SetKey(aValue);
		//_ItemAbility->Silence();
	}
};

const char * ItemMissingCondition::Key = "itemmissing";
