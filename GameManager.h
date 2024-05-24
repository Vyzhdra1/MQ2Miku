#pragma once

#include "StateManager.h"
#include "AbilityManager.h"

class GameManager {
private:
	static StateManager* _StateManager;
public:
	static void Init() {
		_StateManager = new StateManager();
		_StateManager->ClearStates();
	}

	static void Deinit() {
		delete _StateManager;
	}

	static StateManager* GetStateManager() {
		return _StateManager;
	}
};
StateManager* GameManager::_StateManager;