#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbAlternateAbilities.h"
#include "StringParameter.h"

class AlternateAbilitiesContext : public ContextBase
{
private:
	std::map<std::string, DbAlternateAbilities*> _AlternateAbilities;
	std::string _ClassKey = "";
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		AlternateAbilities.AAKey, \
			 		AlternateAbilities.ClassKey, \
			 		AlternateAbilities.ActCode, \
			 		AlternateAbilities.IsBuff \
			 	FROM  \
			 	    AlternateAbilities \
				WHERE \
					AlternateAbilities.ClassKey = ? ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_ClassKey));
		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _AlternateAbilities) {
			delete lValue;
		}

		_AlternateAbilities.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbAlternateAbilities* lAlternateAbility = new DbAlternateAbilities(lRow);
			_AlternateAbilities[lAlternateAbility->GetAAKey()] = lAlternateAbility;
		}
	}
public:
	AlternateAbilitiesContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~AlternateAbilitiesContext() { Clear(); }

	void SetParameters(std::string aClassKey) {
		_ClassKey = aClassKey;
	}

	std::map<std::string, DbAlternateAbilities*>* GetAlternateAbilities() {
		return &_AlternateAbilities;
	}
};


