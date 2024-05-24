#pragma once
#include "Utils.h"
#include "SpellUtils.h"
#include "Condition.h"
#include "StateManager.h"
#include <chrono>
#include <string>

class Action
{
protected:
	Ability* _Ability = 0;
	std::string _Key;

protected:
	bool _CastSuccessful = false;
	bool _IsCasting = false;
	std::vector<Condition*> _Conditions;
	bool _Enabled = true;
public:
	~Action() {
		for (std::vector<Condition*>::iterator lIterator = _Conditions.begin(); lIterator != _Conditions.end(); lIterator++) {
			delete *lIterator;
		}
	}

	void Enable() {
		_Enabled = true;
	}

	void Disable() {
		_Enabled = false;
	}

	virtual bool IsEnabled() {
		return _Enabled;
	}

	bool AbilityFound() {
		return _Ability->AbilityFound();
	}

	bool AbilityReady() {
		return _Ability->AbilityReady();
	}

	Ability * GetAbility() {
		return _Ability;
	}

	void SetAbility(Ability * aAbility) {
		_Ability = aAbility;
	}

	void SetKey(std::string aKey) {
		_Key = aKey;
	}

	const char* GetKey() {
		return _Key.c_str();
	}

	bool IsCastSuccessful() {
		return _CastSuccessful;
	}

	virtual void Reset() {
		_IsCasting = false;
		_CastSuccessful = false;
	}

	void SetCastSuccessful() {
		_CastSuccessful = true;
		_IsCasting = false;
	}

	bool ConditionsMet() {
		for (std::vector<Condition*>::iterator lIterator = _Conditions.begin(); lIterator != _Conditions.end(); lIterator++) {
			if (!(*lIterator)->ConditionMet(_Ability)) return false;
		}
		return true;
	}

	void AddCondition(Condition * aCondition) {
		_Conditions.push_back(aCondition);
	}

	virtual bool CanCastSimultaneously() {
		return _Ability->CanCastSimultaneously();
	}

	virtual bool CanCast() {
		return AbilityReady() && ConditionsMet();
	}

	virtual bool ShouldBeAbleToRecast() {
		return _Ability->ShouldBeAbleToRecast();
	}

	virtual bool ExecuteAbility() {
		if (_CastSuccessful || (!_IsCasting && Utils::IsCasting())) return false;

		if (_IsCasting && _Ability->IsCasting()) {
			SetCastSuccessful();
			return true;
		}

		if (!CanCast()) return false;

		if (!_IsCasting) {
			_IsCasting = true;
			_Ability->EchoCastMessage(Utils::SUCCESS_COLOR, "Casting: ");
		}

		if (_Ability->IsFastCast()) {
			SetCastSuccessful();
		}

		if (!Utils::IsCasting()) {
			_Ability->Cast();
		}

		return true;
	}
};