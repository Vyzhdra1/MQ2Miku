#pragma once
#include "AbilityCollectionAbility.h"
#include "SpawnManager.h"


class XTargetAction : public AbilityCollectionAbility
{
public:
	static const char* Key;

	virtual bool CanCastSimultaneously() {
		return false;
	}

	void UpdateXTargetID(int aXTargetID) {
		for (Condition* lCondition : _Conditions) {
			lCondition->SetXTargetID(aXTargetID);
		}
	}

	bool ExecuteAbility() {
		ExtendedTargetList* lTargetList = GetCharInfo()->pExtendedTargetList;

		if (!lTargetList) return false;

		PlayerClient* lAttackSpawn = SpawnManager::Get()->GetSpawn(ATTACK_TARGET);

		for (ExtendedTargetSlot* lIterator = lTargetList->begin(); lIterator != lTargetList->end(); lIterator++) {
			if (!lIterator) continue;

			if (!lIterator->SpawnID) continue;

			if (lAttackSpawn && (lAttackSpawn->SpawnID == lIterator->SpawnID)) continue;

			PlayerClient* lTarget = GetSpawnByID(lIterator->SpawnID);

			if (!lTarget) continue;

			if (lTarget->Type != SPAWN_NPC) continue;

			if (ExecuteAbility(lTarget)) return true;
		}

		Reset();
		return false;
	}

	bool ExecuteAbility(PlayerClient* aXTarget) {
		UpdateXTargetID(aXTarget->SpawnID);

		if (!ConditionsMet()) {
			return false;
		}

		for (std::vector<Action*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			if ((*lIterator)->AbilityReady() && (*lIterator)->ConditionsMet()) {
				Utils::MikuEcho(Utils::BLUE_COLOR, "Targeted: ", aXTarget->Name);
				Utils::TargetByID(GetCharInfo()->pSpawn, aXTarget->SpawnID);
				return CastSelectedAbility(*lIterator);
			}
		}
		return false;
	}
};

const char* XTargetAction::Key = "xtarget";