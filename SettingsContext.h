#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbSettings.h"
#include "StringParameter.h"

class SettingsContext: public ContextBase
{
private:
	std::map<std::string, DbSettings*> _Settings;
	std::string _ClassKey = "";
protected:
	std::string GetQueryString() override  {
		return
			"	SELECT \
			 		Settings.Name, \
			 		Settings.Value, \
			 		Settings.MinValue, \
			 		Settings.MaxValue, \
					Settings.ValueType, \
			 		Settings.ValidStrings \
			 	FROM  \
			 	    Settings \
				WHERE \
					Settings.Classes = ? \
					OR Settings.Classes LIKE '%' || ? || '%' ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter("ALL"));
		lResult.push_back(new StringParameter(_ClassKey));
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
			DbSettings* lSetting = new DbSettings(lRow);
			_Settings[lSetting->GetName()] = lSetting;
		}
	}
public:
	SettingsContext(DbConnection * aConnection) : ContextBase(aConnection) {} 
	~SettingsContext() { Clear(); }

	bool HasSetting(std::string aName) {
		return _Settings.find(aName) != _Settings.end();
	}

	void SetParameters(std::string aClassKey) {
		_ClassKey = aClassKey;
	}

	DbSettings* GetSetting(std::string aName) {
		if (!HasSetting(aName)) return 0;
		return _Settings[aName];
	}

	std::map<std::string, DbSettings*> * GetSettings() {
		return &_Settings;
	}
};


