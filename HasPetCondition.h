#ifndef H_HasPetCondition
#define H_HasPetCondition
#include "Ability.h"
#include "BooleanCondition.h"
#include "PlayerUtils.h"
#include <set>

class HasPetCondition : public BooleanCondition
{
public:
	static const char* Key;

	HasPetCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return PlayerUtils::HasPet() == _BooleanCondition;
	}
};
const char* HasPetCondition::Key = "haspet";

#endif