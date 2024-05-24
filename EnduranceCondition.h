#ifndef H_EnduranceCondition
#define H_EnduranceCondition
#include "PlayerUtils.h"
#include "SpawnCondition.h"

class EnduranceCondition : public SpawnCondition
{
public:
	static const char * Key;

	EnduranceCondition(std::set<std::string> aSettings) : SpawnCondition(aSettings) {}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) {
		return (PlayerUtils::ValuePerc(aSpawn->EnduranceMax, aSpawn->EnduranceCurrent) >= _IntegerValue) == _BooleanCondition;
	}
};

const char * EnduranceCondition::Key = "end>";

#endif