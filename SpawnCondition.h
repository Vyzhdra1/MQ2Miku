#pragma once
#include "Ability.h"
#include "Condition.h"
#include "SpawnManager.h"
#include "BooleanCondition.h"
#include "Utils.h"
class SpawnCondition : public BooleanCondition
{
protected:
	SpawnType _SpawnType = UNKNOWN;
	int _XTargetID = 0;
	int _IntegerValue;
public:
	static const char* Key;

	SpawnCondition(std::set<std::string> aSettings) : BooleanCondition(aSettings) {
		Load(aSettings);
	}

	bool ConditionMet(Ability* aAbility) {
		PlayerClient* lSpawn = 0;
		if (_SpawnType == SpawnType::XTARGET) {
			lSpawn = GetSpawnByID(_XTargetID);
		}
		else {
			lSpawn = SpawnManager::Get()->GetSpawn(_SpawnType);
		}

		if (!lSpawn) return false;

		return UnderlyingConditionMet(aAbility, lSpawn);
	}

	virtual void ParseNextValue(std::string aValue) override {
		if (_SpawnType == UNKNOWN) {
			_SpawnType = PlayerUtils::StringToSpawnType(aValue);
		}

		if (_SpawnType == UNKNOWN) {
			_IntegerValue = Utils::StrToInt(aValue.c_str(), -1);

			if (_IntegerValue == -1) {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
			}
		}
	}

	virtual bool UnderlyingConditionMet(Ability* aAbility, PlayerClient* aSpawn) = 0;

	virtual SpawnType GetSpawnType() override {
		return _SpawnType;
	}

	virtual void SetXTargetID(int aXTargetID) override { 
		_XTargetID = aXTargetID;
	}
};