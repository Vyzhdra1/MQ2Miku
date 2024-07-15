#pragma once
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"
#include <set>

class ItemOnCursorCondition : public BooleanCondition
{
private:
	bool _UseSummonedList = false;
	std::set<int> _SummonedAxe{
		159964, //Axe121
		124360, //Axe116 };
		150401,  //Axe111
		93700  //Axe106
	};
	std::string _Modulation = "Modulation";
public:
	static const char* Key;

	ItemOnCursorCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	virtual void ParseNextValue(std::string aValue) override {
		if (!aValue.compare("summonedlist")) {
			_UseSummonedList = true;
			return;
		}
		Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
	}

	bool ConditionMet(Ability* aAbility) {
		if (GetPcProfile()->GetInventorySlot(InvSlot_Cursor) == nullptr) return _BooleanCondition == false;

		if (!_UseSummonedList) return _BooleanCondition == true;

		ItemPtr lItem = GetPcProfile()->GetInventorySlot(InvSlot_Cursor);
		if (_SummonedAxe.count(lItem->ID)) return _BooleanCondition == true;

		std::string lItemName = std::string(lItem->GetName());
		if (lItemName.find(_Modulation) != std::string::npos) return _BooleanCondition == true;

		return false;
	}
};
const char* ItemOnCursorCondition::Key = "cursoritem";
