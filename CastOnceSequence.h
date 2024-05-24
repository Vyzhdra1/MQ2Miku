#ifndef H_CastOnceSequence
#define H_CastOnceSequence
#include "AbilityCollectionAbility.h"

class CastOnceSequence : public AbilityCollectionAbility
{
public:
	static const char* Key;

	bool ExecuteAbility() {
		if (!ConditionsMet()) {
			return false;
		}

		bool lAbilityReadyExists = false;
		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			lAbilityReadyExists = lAbilityReadyExists || ((*lIterator)->AbilityReady() && CanCastSelectedAbility(*lIterator));
			if (lAbilityReadyExists && CastSelectedAbility(*lIterator)) {
				if (!CanCastSimultaneously(*lIterator)) {
					return true;
				}
			}
		}
		if (!lAbilityReadyExists) {
			Disable();
		}
		return false;
	}
};
const char* CastOnceSequence::Key = "onceoff";

#endif