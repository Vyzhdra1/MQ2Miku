#pragma once

#include "Ability.h"
#include "AbilityCollectionAbility.h"
#include "PlayerUtils.h"
#include "PlayerUtils.h"
#include <set>

class SpellBuffAction : public AbilityCollectionAbility
{
private:
	int _MemoriseOffset = 2000;
	unsigned long _MemorizeTimer = Utils::GetClockTime();
public:
	static const char* Key;

	virtual bool CanCastSimultaneously() {
		return false;
	}

	bool ExecuteAbility() {
		if (_SpawnType == UNKNOWN) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Target for: ", GetKey());
			return false;
		}

		if (!ConditionsMet()) {
			return false;
		}

		PlayerClient* lSpawn = GetSpawn();;

		if (!lSpawn) return false;

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if (!(*lIterator)->ConditionsMet()) continue;

			if (!(*lIterator)->GetAbility()->Memorized(true)) {
				if (Utils::GetClockTime() < _MemorizeTimer) return true;

				(*lIterator)->GetAbility()->Memorize(13);
				_MemorizeTimer = Utils::GetClockTime() + _MemoriseOffset;
				return true;
			}

			Utils::MikuEcho(Utils::BLUE_COLOR, "Check Ability Ready: ", lSpawn->Name);
			if (!(*lIterator)->AbilityReady()) return true;

			Utils::MikuEcho(Utils::BLUE_COLOR, "Targeted: ", lSpawn->Name);
			return CastSelectedAbility(*lIterator);
		}
		Reset();
		return false;
	}
};
const char* SpellBuffAction::Key = "spellbuff";