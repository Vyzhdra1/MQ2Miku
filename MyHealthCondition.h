#ifndef H_MyHealthConditon
#define H_MyHealthConditon
#include "PlayerUtils.h"
#include "EssenceCondition.h"

class MyHealthCondition : public EssenceCondition
{
public:
	static const char * Key;

	MyHealthCondition(std::set<std::string> aSettings) : EssenceCondition(aSettings) {}

	int64_t GetValuePerc(Ability* aAbility) {
		return PlayerUtils::HealthPerc();
	}
};

const char * MyHealthCondition::Key = "hp>";

#endif