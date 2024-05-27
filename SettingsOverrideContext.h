#pragma once
#include <map>
#include <string>
#include <type_traits>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbSettingsOverride.h"
#include "IntParameter.h"

class SettingsOverrideContext : public ContextBase
{
private:
	std::map<std::string, DbSettingsOverride*> _Settings;
	int _CharacterID = -1;
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		SettingsOverride.Name, \
			 		SettingsOverride.Value, \
			 		SettingsOverride.CharacterID, \
					Settings.ValueType \
			 	FROM  \
			 	    SettingsOverride  \
				INNER JOIN Settings USING (Name) \
				WHERE \
					SettingsOverride.CharacterId = ? ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new IntParameter(_CharacterID));

		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _Settings) {
			delete lValue;
		}

		_Settings.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbSettingsOverride* lSetting = new DbSettingsOverride(lRow);
			Add(lSetting->GetName(), lSetting);
		}
	}
public:
	SettingsOverrideContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~SettingsOverrideContext() { Clear(); }

	bool HasSetting(std::string aName) {
		return _Settings.find(aName) != _Settings.end();
	}

	DbSettingsOverride* GetSetting(std::string aName) {
		if (!HasSetting(aName)) return 0;
		return _Settings[aName];
	}

	void SetParameters(int aCharacterID) {
		_CharacterID = aCharacterID;
	}

	int GetCharacterID() {
		return _CharacterID;
	}

	std::map<std::string, DbSettingsOverride*>* GetSettings() {
		return &_Settings;
	}

	void Add(std::string aKey, DbSettingsOverride* aValue) {
		_Settings[aKey] = aValue;
	}
};


