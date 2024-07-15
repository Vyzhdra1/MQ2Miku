#ifndef H_Condition
#define H_Condition
#include "Ability.h"
#include "GlobalKeys.h"

class Condition
{
public:
	virtual bool ConditionMet(Ability * aAbility) = 0;

	void ParseTrueFalseSet(std::set<std::string> aSettings, bool &pBoolCondition, int &pIntValue) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			if (!TRUE_STR.compare((*lIterator).c_str())) {
				pBoolCondition = true;
			}
			else if (!FALSE_STR.compare((*lIterator).c_str())) {
				pBoolCondition = false;
			}
			else {
				pIntValue = Utils::StrToInt((*lIterator).c_str(), 0);

				if (pIntValue == 0) {
					Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", (*lIterator));
				}
			}
		}
	}

	virtual SpawnType GetSpawnType() {
		return SpawnType::UNKNOWN;
	}

	virtual void SetXTargetID(int XTargetID) { }
};

#endif