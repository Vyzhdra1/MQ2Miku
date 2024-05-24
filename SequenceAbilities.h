#ifndef H_SequenceAbilities
#define H_SequenceAbilities
#include "AbilityCollectionAbility.h"

class SequenceAbilities : public AbilityCollectionAbility
{
public:
	static const char* Key;

	bool ExecuteAbility() {
		if (!ConditionsMet()) {
			return false;
		}

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (CanCastSelectedAbility(*lIterator) && CastSelectedAbility(*lIterator)) {
				if (!CanCastSimultaneously(*lIterator)) {
					return true;
				}
			}
		}
		Reset();
		return false;
	}
};
const char* SequenceAbilities::Key = "sequence";

#endif