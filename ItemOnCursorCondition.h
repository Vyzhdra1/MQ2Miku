#pragma once
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"
#include <set>

class ItemOnCursorCondition : public BooleanCondition
{
public:
	static const char* Key;

	ItemOnCursorCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return (GetPcProfile()->GetInventorySlot(InvSlot_Cursor) != nullptr) == _BooleanCondition;
	}
};
const char* ItemOnCursorCondition::Key = "cursoritem";
