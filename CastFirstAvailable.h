#ifndef H_CASTFIRSTAVAILABLEABILITY
#define H_CASTFIRSTAVAILABLEABILITY
#include "AbilityCollectionAbility.h"

class CastFirstAvailableAbility : public AbilityCollectionAbility
{
public:
	static const char * Key;

	bool ExecuteAbility() {
		if (!ConditionsMet()) {
			return false;
		}

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (CanCastSelectedAbility(*lIterator) && CastSelectedAbility(*lIterator)) {
				Disable();
				return true;
			}
		}
		return true;
	}
};

const char * CastFirstAvailableAbility::Key = "firstavailable";

#endif