#ifndef H_Factory
#define H_Factory

#include "Ability.h"
#include "SpellAbility.h"
#include "ItemAbility.h"
#include "AAAbility.h"
#include "DiscAbility.h"
#include "SkillAbility.h"
#include "RemoveAuraAbility.h"
#include "CommandAbility.h"

#include "Parser.h"
#include "MikuPlayer.h"
#include "Utils.h"
#include "Entry.h"
#include "Token.h"
#include "ConditionFactory.h"

#include "BuffAbilities.h"
#include "SequenceAbilities.h"
#include "CastOnceSequence.h"
#include "CastFirstAvailable.h"
#include "TargetAction.h"
#include "XTargetAction.h"
#include "SpellBuffAction.h"
#include "ScribeSetAction.h"

class Factory
{
public:
	static AbilityCollectionAbility* BuildAbilityCollection(Entry* aEntry) {
		AbilityCollectionAbility* lResult = 0;

		if (!aEntry->GetType().compare("")) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Type parameter!");
			return lResult;
		}

		if (!aEntry->GetType().compare(SequenceAbilities::Key)) {
			lResult = new SequenceAbilities();
		}
		else if (!aEntry->GetType().compare(BuffAbilities::Key)) {
			lResult = new BuffAbilities();
		}
		else if (!aEntry->GetType().compare(CastOnceSequence::Key)) {
			lResult = new CastOnceSequence();
		}
		else if (!aEntry->GetType().compare(CastFirstAvailableAbility::Key)) {
			lResult = new CastFirstAvailableAbility();
		}
		else if (!aEntry->GetType().compare(TargetAction::Key)) {
			lResult = new TargetAction();
		}
		else if (!aEntry->GetType().compare(SpellBuffAction::Key)) {
			lResult = new SpellBuffAction();
		}
		else if (!aEntry->GetType().compare(XTargetAction::Key)) {
			lResult = new XTargetAction();
		}
		else if (!aEntry->GetType().compare(ScribeSetAction::Key)) {
			lResult = new ScribeSetAction();
		}

		if (lResult) {
			lResult->SetKey(aEntry->GetID());
			lResult->SetSpawnType(aEntry->GetTarget());
			lResult->SetTargetID(aEntry->GetTargetID());
			lResult->SetAllowEmpty(aEntry->GetAllowEmpty());
			lResult->SetCastCounter(aEntry->GetCastLimit()); 
			lResult->SetRemoveTime(aEntry->GetTimer());
			if (aEntry->IsForced()) {
				lResult->ForcePermenentlyEnabled();
			}
			ConditionFactory::AddConditions(aEntry, lResult);
		}
		else {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid Type Specified: ", aEntry->GetType());
		}

		return lResult;
	}

	static Ability* BuildAbility(Entry* aEntry, MikuPlayer* aPlayer) {
		Ability* lResult = 0;

		if (!aEntry->GetType().compare("")) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Type parameter!");
			return lResult;
		}

		if (!aEntry->GetType().compare(SpellAbility::ConfigKey)) {
			lResult = new SpellAbility();
		}
		else if (!aEntry->GetType().compare(DiscAbility::ConfigKey)) {
			lResult = new DiscAbility();
		}
		else if (!aEntry->GetType().compare(ItemAbility::ConfigKey)) {
			lResult = new ItemAbility();
		}
		else if (!aEntry->GetType().compare(SkillAbility::ConfigKey)) {
			lResult = new SkillAbility();
		}
		else if (!aEntry->GetType().compare(AAAbility::ConfigKey)) {
			lResult = new AAAbility();
		}
		else if (!aEntry->GetType().compare(RemoveAuraAbility::ConfigKey)) {
			lResult = new RemoveAuraAbility();
		}
		else if (!aEntry->GetType().compare(CommandAbility::ConfigKey)) {
			lResult = new CommandAbility();
		}

		if (!lResult) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid Type Specified: ", aEntry->GetType());
			return lResult;
		}

		lResult->SetKey(aEntry->GetKey());
		lResult->SetName(aEntry->GetID());
		lResult->SetMatchType(aEntry->GetMatchType());
		lResult->SetModLevel(aEntry->GetLevelMod());

		return lResult;
	}
};
#endif