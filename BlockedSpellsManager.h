#pragma once

class BlockedSpellsManager {
private:
	inline static BlockedSpellsManager* _Manager = 0;
	std::set<int> _BlockedSpellIDs;
	std::set<int> _BlockedPetSpellIDs;

	void LoadBlockedSpellIDs() {
		_BlockedSpellIDs.clear();
		for (auto i = 0; i < MAX_BLOCKED_SPELLS; ++i) {
			_BlockedSpellIDs.insert(pLocalPC->BlockedSpell[i]);
		}
	}

	void LoadBlockedPetSpellIDs() {
		_BlockedPetSpellIDs.clear();
		for (auto i = 0; i < MAX_BLOCKED_SPELLS_PET; ++i) {
			_BlockedPetSpellIDs.insert(pLocalPC->BlockedPetSpell[i]);
		}
	}

public:
	void Load() {
		LoadBlockedSpellIDs();
		LoadBlockedPetSpellIDs();
	}

	bool IsSpellBlocked(int aID) {
		return _BlockedSpellIDs.count(aID) > 0;
	}

	bool IsSpellBlockedPet(int aID) {
		return _BlockedPetSpellIDs.count(aID) > 0;
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static BlockedSpellsManager* Get() {
		if (!_Manager) {
			_Manager = new BlockedSpellsManager();
			_Manager->Load();
		}
		return _Manager;
	}
};

