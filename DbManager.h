#pragma once
#include <string>
#include <map>
#include <mq/Plugin.h>
#include "Utils.h"
#include "DbConnection.h"
#include "SettingsContext.h"
#include "SettingsOverrideContext.h"
#include "DbCharacter.h"
#include "MikuPlayer.h"

class DbManager
{
private:
	inline static DbManager* _Workspace = 0;
	DbConnection* _Connection = 0;
	DbCharacter* _Character = 0;
	std::map<std::string, StoredObject*> _WorkspaceCache;
	SettingsContext* _SettingsContext = 0;
	SettingsOverrideContext* _SettingsOverrideContext = 0;
public:
	DbManager() {
		_Connection = new DbConnection();
		_Connection->OpenDatabase();
		_SettingsContext = new SettingsContext(_Connection);
		_SettingsOverrideContext = new SettingsOverrideContext(_Connection);
	}

	~DbManager() {
		if (_Connection) {
			delete _Connection;
		}

		if (_Character) {
			delete _Character;
		}

		delete _SettingsContext;
		delete _SettingsOverrideContext;
	}

	void Save(StoredObject* aObj) {
		aObj->Save(_Connection);
	}

	void Load(StoredObject* aObj) {
		aObj->Load(_Connection);
	}

	void AssignDbCharacter(DbCharacter* aCharacter) {
		_Character = aCharacter;
	}

	DbCharacter* GetCharacter() {
		return _Character;
	}

	SettingsContext* GetSettingsContext() {
		return _SettingsContext;
	}

	SettingsOverrideContext* GetSettingsOverrideContext() {
		return _SettingsOverrideContext;
	}

	static void Deinit() {
		if (_Workspace) {
			delete _Workspace;
			_Workspace = 0;
		}
	}

	static DbManager* Get() {
		if (!_Workspace) {
			_Workspace = new DbManager();
		}
		return _Workspace;
	}
};