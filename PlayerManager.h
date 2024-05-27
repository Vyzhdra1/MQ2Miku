#pragma once
#include <string>
#include <map>
#include <mq/Plugin.h>
#include "Utils.h"
#include "DbConnection.h"
#include "SettingsContext.h"
#include "SettingsOverrideContext.h"
#include "CharacterContext.h"

class PlayerManager
{
private:
	inline static PlayerManager* _PlayerManager = 0;
	DbCharacter* _Character = 0;
public:
	PlayerManager() {}

	~PlayerManager() {	}

	void UpdatePlayerZone() {

	}

	void UpdateBuffStatus() {

	}

	static void Deinit() {
		if (_PlayerManager) {
			delete _PlayerManager;
			_PlayerManager = 0;
		}
	}

	static PlayerManager* Get() {
		if (!_PlayerManager) {
			_PlayerManager = new PlayerManager();
		}
		return _PlayerManager;
	}
};