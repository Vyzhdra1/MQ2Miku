#pragma once

#include "Condition.h"
#include "RecastDebuffCondition.h"
#include "RecastBuffCondition.h"
#include "ActiveDiscCondition.h"
#include "HealthCondition.h"
#include "ManaCondition.h"
#include "CombatCondition.h"
#include "EnduranceCondition.h"
#include "BuffActiveCondition.h"
#include "ItemMissingCondition.h"
#include "HasPetCondition.h"
#include "StandingCondition.h"
#include "InvisibleCondition.h"
#include "StateCondition.h"
#include "ItemOnCursorCondition.h"
#include "MovingCondition.h"
#include "CastingCondition.h"
#include "TimerCondition.h"
#include "DeadPlayerCondition.h"
#include "XTarRangeCondition.h"
#include "SettingCondition.h"
#include "BuffCondition.h"
#include "BuffExistsCondition.h"
#include "AbilityReadyCondition.h"
#include "BannedTargetCondition.h"
#include "MezzedCondition.h"


class ConditionFactory
{
public:
	static void AddConditions(Entry* aEntry, Action* aAbility) {
		std::set<Token*> lConditions = aEntry->GetConditions();

		for (std::set<Token*>::iterator lIterator = lConditions.begin(); lIterator != lConditions.end(); lIterator++) {
			std::string lCondtitionName = (*lIterator)->GetKey();
			std::set<std::string> lSettings = (*lIterator)->GetSettings();

			if (!lCondtitionName.compare(RecastDebuffCondition::Key)) {
				aAbility->AddCondition(new RecastDebuffCondition(lSettings));
			}
			else if (!lCondtitionName.compare(ActiveDiscCondition::Key)) {
				aAbility->AddCondition(new ActiveDiscCondition(lSettings));
			}
			else if (!lCondtitionName.compare(RecastBuffCondition::Key)) {
				aAbility->AddCondition(new RecastBuffCondition(lSettings));
			}
			else if (!lCondtitionName.compare(HealthCondition::Key)) {
				aAbility->AddCondition(new HealthCondition(lSettings));
			}
			else if (!lCondtitionName.compare(CombatCondition::Key)) {
				aAbility->AddCondition(new CombatCondition(lSettings));
			}
			else if (!lCondtitionName.compare(EnduranceCondition::Key)) {
				aAbility->AddCondition(new EnduranceCondition(lSettings));
			}
			else if (!lCondtitionName.compare(BuffActiveCondition::Key)) {
				aAbility->AddCondition(new BuffActiveCondition(lSettings));
			}
			else if (!lCondtitionName.compare(ManaCondition::Key)) {
				aAbility->AddCondition(new ManaCondition(lSettings));
			}
			else if (!lCondtitionName.compare(ItemMissingCondition::Key)) {
				aAbility->AddCondition(new ItemMissingCondition(lSettings));
			}
			else if (!lCondtitionName.compare(HasPetCondition::Key)) {
				aAbility->AddCondition(new HasPetCondition(lSettings));
			}
			else if (!lCondtitionName.compare(StandingCondition::Key)) {
				aAbility->AddCondition(new StandingCondition(lSettings));
			}
			else if (!lCondtitionName.compare(InvisibleCondition::Key)) {
				aAbility->AddCondition(new InvisibleCondition(lSettings));
			}
			else if (!lCondtitionName.compare(StateCondition::Key)) {
				aAbility->AddCondition(new StateCondition(lSettings));
			}
			else if (!lCondtitionName.compare(ItemOnCursorCondition::Key)) {
				aAbility->AddCondition(new ItemOnCursorCondition(lSettings));
			}
			else if (!lCondtitionName.compare(MovingCondition::Key)) {
				aAbility->AddCondition(new MovingCondition(lSettings));
			}
			else if (!lCondtitionName.compare(CastingCondition::Key)) {
				aAbility->AddCondition(new CastingCondition(lSettings));
			}
			else if (!lCondtitionName.compare(TimerCondition::Key)) {
				aAbility->AddCondition(new TimerCondition(lSettings));
			}
			else if (!lCondtitionName.compare(DeadPlayerCondition::Key)) {
				aAbility->AddCondition(new DeadPlayerCondition(lSettings));
			}
			else if (!lCondtitionName.compare(XTarRangeCondition::Key)) {
				aAbility->AddCondition(new XTarRangeCondition(lSettings));
			}
			else if (!lCondtitionName.compare(SettingCondition::Key)) {
				aAbility->AddCondition(new SettingCondition(lSettings));
			}
			else if (!lCondtitionName.compare(BuffCondition::Key)) {
				aAbility->AddCondition(new BuffCondition(lSettings));
			}
			else if (!lCondtitionName.compare(BuffExistsCondition::Key)) {
				aAbility->AddCondition(new BuffExistsCondition(lSettings));
			}
			else if (!lCondtitionName.compare(AbilityReadyCondition::Key)) {
				aAbility->AddCondition(new AbilityReadyCondition(lSettings));
			}
			else if (!lCondtitionName.compare(BannedTargetCondition::Key)) {
				aAbility->AddCondition(new BannedTargetCondition(lSettings));
			}
			else if (!lCondtitionName.compare(MezzedCondition::Key)) {
				aAbility->AddCondition(new MezzedCondition(lSettings));
			}
			else {				
				Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown Condition: ", lCondtitionName);
			}
		}
	}
}; 