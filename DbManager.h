#pragma once
#include <string>
#include <map>
#include <mq/Plugin.h>
#include "Utils.h"
#include "DbConnection.h"
#include "StoredObject.h"
#include "SettingsContext.h"
#include "GearContext.h"
#include "SettingsOverrideContext.h"
#include "SpellsContext.h"
#include "ItemAbilityContext.h"
#include "AlternateAbilitiesContext.h"
#include "DbCharacter.h"
#include "CommandContext.h"
#include "SpellSetContext.h"
#include "GroupContext.h"
#include "GuildContext.h"

enum DbContextType { 
	SETTINGSCONTEXT, 
	SETTINGSOVERRIDECONTEXT, 
	SPELLCONTEXT, 
	GEARCONTEXT, 
	ALTERNATEABILITIESCONTEXT,
	COMMANDCONTEXT,
	ITEMABILITYCONTEXT,
	SPELLSETCONTEXT,
	GROUPCONTEXT,
	GUILDCONTEXT
};

class DbManager
{
private:
	inline static DbManager* _Workspace = 0;
	DbConnection* _Connection = 0;
	DbCharacter* _Character = 0;
	std::map<std::string, StoredObject*> _WorkspaceCache;
	std::map<DbContextType, ContextBase*> _Contexts;
public:
	DbManager() {
		_Connection = new DbConnection();
		_Connection->OpenDatabase();
	}

	~DbManager() {
		if (_Connection) {
			delete _Connection;
		}

		if (_Character) {
			delete _Character;
		}

		for (const auto& [lKey, lValue] : _Contexts) {
			delete lValue;
		}
		_Contexts.clear();
	}

	void Save(StoredObject* aObj) {
		aObj->Save(_Connection);
	}

	void Load(StoredObject* aObj) {
		aObj->Load(_Connection);
	}

	void ExecSql(Query* aQuery) {
		_Connection->QueryDatabasePrepare(aQuery);
	}

	void AssignDbCharacter(DbCharacter* aCharacter) {
		_Character = aCharacter;
	}

	DbCharacter* GetCharacter() {
		return _Character;
	}

	SettingsContext* GetSettingsContext() {
		if (!_Contexts.count(SETTINGSCONTEXT)) {
			_Contexts[SETTINGSCONTEXT] = new SettingsContext(_Connection);
		}
		return (SettingsContext*) _Contexts[SETTINGSCONTEXT];
	}

	SettingsOverrideContext* GetSettingsOverrideContext() {
		if (!_Contexts.count(SETTINGSOVERRIDECONTEXT)) {
			_Contexts[SETTINGSOVERRIDECONTEXT] = new SettingsOverrideContext(_Connection);
		}
		return (SettingsOverrideContext*)_Contexts[SETTINGSOVERRIDECONTEXT];
	}

	SpellsContext* GetSpellsContext() {
		if (!_Contexts.count(SPELLCONTEXT)) {
			_Contexts[SPELLCONTEXT] = new SpellsContext(_Connection);
		}
		return (SpellsContext*)_Contexts[SPELLCONTEXT];
	}

	GearContext* GetGearContext() {
		if (!_Contexts.count(GEARCONTEXT)) {
			_Contexts[GEARCONTEXT] = new GearContext(_Connection);
		}
		return (GearContext*)_Contexts[GEARCONTEXT];
	}
	
	AlternateAbilitiesContext* GetAlternateAbilitiesContext() {
		if (!_Contexts.count(ALTERNATEABILITIESCONTEXT)) {
			_Contexts[ALTERNATEABILITIESCONTEXT] = new AlternateAbilitiesContext(_Connection);
		}
		return (AlternateAbilitiesContext*)_Contexts[ALTERNATEABILITIESCONTEXT];
	}

	CommandContext* GetCommandContext() {
		if (!_Contexts.count(COMMANDCONTEXT)) {
			_Contexts[COMMANDCONTEXT] = new CommandContext(_Connection);
		}
		return (CommandContext*)_Contexts[COMMANDCONTEXT];
	}

	ItemAbilityContext* GetItemAbilityContext() {
		if (!_Contexts.count(ITEMABILITYCONTEXT)) {
			_Contexts[ITEMABILITYCONTEXT] = new ItemAbilityContext(_Connection);
		}
		return (ItemAbilityContext*)_Contexts[ITEMABILITYCONTEXT];
	}

	SpellSetContext* GetSpellSetContext() {
		if (!_Contexts.count(SPELLSETCONTEXT)) {
			_Contexts[SPELLSETCONTEXT] = new SpellSetContext(_Connection);
		}
		return (SpellSetContext*)_Contexts[SPELLSETCONTEXT];
	}

	GroupContext* GetGroupContext() {
		if (!_Contexts.count(GROUPCONTEXT)) {
			_Contexts[GROUPCONTEXT] = new GroupContext(_Connection);
		}
		return (GroupContext*)_Contexts[GROUPCONTEXT];
	}

	GuildContext* GetGuildContext() {
		if (!_Contexts.count(GUILDCONTEXT)) {
			_Contexts[GUILDCONTEXT] = new GuildContext(_Connection);
		}
		return (GuildContext*)_Contexts[GUILDCONTEXT];
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