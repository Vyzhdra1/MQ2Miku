#pragma once
#include "BooleanCondition.h"
#include "PlayerUtils.h"

class XTarRangeCondition : public BooleanCondition
{
private:
	int _IntegerValue = -1;
public:
	static const char* Key;

	XTarRangeCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		return XTargetsInRange() == _BooleanCondition;
	}

	virtual void ParseNextValue(std::string aValue) override {
		_IntegerValue = Utils::StrToInt(aValue.c_str(), -1);

		if (_IntegerValue == -1) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
		}
	}

	bool XTargetsInRange() {
		ExtendedTargetList* lTargetList = GetCharInfo()->pExtendedTargetList;

		if (!lTargetList) return false;

		int lCount = 0;

		for (ExtendedTargetSlot* lIterator = lTargetList->begin(); lIterator != lTargetList->end(); lIterator++) {
			if (!lIterator) continue;

			if (!lIterator->SpawnID) continue;

			PlayerClient* lTarget = GetSpawnByID(lIterator->SpawnID);

			if (!lTarget) continue;

			if (lTarget->Type != SPAWN_NPC) continue;

			int lNewDistance = (int) DistanceToSpawn(GetCharInfo()->pSpawn, lTarget);

			if (lNewDistance < _IntegerValue) {
				lCount++;
			}

			if (lCount >= 2) {
				return true;
			}
		}

		return false;
	}
};

const char* XTarRangeCondition::Key = "xtar";

