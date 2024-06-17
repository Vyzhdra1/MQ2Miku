#pragma once
#include <map>
#include <string>
#include "Ability.h"
#include "SpellAbility.h"
#include "DiscAbility.h"

class AbilityManager {
private:
	inline static AbilityManager* _Manager = 0;
	std::map<std::string, Ability*> _Abilities;

public:
	void AddAbility(std::string aKey, Ability* aAbility) {
		if (_Abilities.count(aKey) > 0) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "CLASH FOUND LOADING KEY: ", aKey);

			delete aAbility;

			return;
		}

		_Abilities[aKey] = aAbility;
	}

	Ability* GetAbility(std::string aKey) {
		if (_Abilities.find(aKey) == _Abilities.end()) return 0;
		return _Abilities[aKey];
	}
	
	void Refresh() {
		for (std::map<std::string, Ability*>::iterator lIterator = _Abilities.begin(); lIterator != _Abilities.end(); lIterator++) {
			lIterator->second->Refresh();
		}
	}

	~AbilityManager() {
		std::map<std::string, Ability*>::iterator lIterator = _Abilities.begin();
		while (lIterator != _Abilities.end()) {
			delete lIterator->second;
			lIterator++;
		}

		_Abilities.clear();
	}

	void ReportLowSpells() {
		std::map<std::string, Ability*>::iterator lIterator = _Abilities.begin();
		while (lIterator != _Abilities.end()) {
			if (dynamic_cast<SpellAbility*>(lIterator->second) || dynamic_cast<DiscAbility*>(lIterator->second)) {
				PSPELL lSpell = lIterator->second->GetSpell();

				if (!lSpell) continue;

				std::string lAbilityType = dynamic_cast<SpellAbility*>(lIterator->second) ? " (Spell)" : " (Disc)";

				if (lSpell->ClassLevel[GetPcProfile()->Class] <= GetPcProfile()->Level - 5) {
					Utils::MikuEcho(Utils::MAGENTA_COLOR, std::to_string(lSpell->ClassLevel[GetPcProfile()->Class]) + lAbilityType, lSpell->Name);
				}

			}
			lIterator++;
		}
	}

	void ReportAbilities() {
		for (const auto& [lKey, lValue] : _Abilities) {
			if (lValue->IsLoaded()) {
				lValue->EchoLoadSuccessMessage();
			}
			else {
				Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to load: ", lKey);
			}
		}
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static AbilityManager* Get() {
		if (!_Manager) {
			_Manager = new AbilityManager();
		}
		return _Manager;
	}
};