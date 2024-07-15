#pragma once
#include "Ability.h"
#include "ItemAbility.h"
#include "BooleanCondition.h"

class ItemMissingCondition : public BooleanCondition
{	
private:
	ItemAbility* _ItemAbility = 0;
	int _StackCount = 0;
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

		return (!_ItemAbility->AbilityFound() || (_ItemAbility->GetStackCount() < _StackCount)) == _BooleanCondition;
	}

	virtual void ParseNextValue(std::string aValue) override {
		int lStackValue = Utils::StrToInt(aValue.c_str(), 0);

		if (lStackValue) {
			_StackCount = lStackValue;
			return;
		}

		if (_ItemAbility) delete _ItemAbility;

		Ability* lAbility = AbilityManager::Get()->GetAbility(aValue);

		if (!lAbility) return;
		if (!dynamic_cast<ItemAbility*>(lAbility)) return;

		_ItemAbility = (ItemAbility*)lAbility;
	}
};

const char * ItemMissingCondition::Key = "itemmissing";
