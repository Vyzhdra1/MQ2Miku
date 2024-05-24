#ifndef H_MovementBehaviour
#define H_MovementBehaviour
#include "Condition.h"
#include "AbilityCollection.h"

class MovementBehaviour : public Condition, public AbilityCollection
{
	enum {NONE, FOLLOW_TANK, STAY_AT_CAMP, FOLLOW_ASSIST};
public:
	bool SelectedDiscActive(Ability* aDisc) {
		return Utils::ActiveDiscipline() == aDisc->GetID();
	}

	void AddAbility(char* aName, Ability* aAbility) {
		aAbility->SetKey(aName);

		if (!aAbility->AbilityFound()) {
			delete aAbility;
		}
		else {
			AbilityCollection::AddAbility(aAbility);
		}
	}

	bool ConditionMet(Ability* aAbility) {
		for (std::vector<Ability*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if ((*lIterator)->AbilityReady()) return true;
		}
		return false;
	}
};

#endif