#ifndef H_ManaCondition
#define H_ManaCondition
#include "PlayerUtils.h"
#include "SpawnCondition.h"

class ManaCondition : public SpawnCondition
{
public:
	static const char* Key;

	ManaCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) {
		return (PlayerUtils::ValuePerc(aSpawn->ManaMax, aSpawn->ManaCurrent) >= _IntegerValue) == _BooleanCondition;
	}
};
const char * ManaCondition::Key = "mana>";

#endif