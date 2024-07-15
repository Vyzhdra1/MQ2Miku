#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbSpellSetLink.h"
#include "StringParameter.h"

class SpellSetContext : public ContextBase
{
private:
	std::map<std::string, std::vector<DbSpellSetLink*>> _SpellSets;
	std::string _ClassKey = "";
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		SpellSets.Name, \
			 		SpellSetLink.GemIndex, \
			 		SpellSetLink.SpellKey \
			 	FROM  \
			 	    SpellSets \
				INNER JOIN SpellSetLink USING (SpellSetID) \
				WHERE \
					SpellSets.ClassKey = ? ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_ClassKey));
		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _SpellSets) {
			for (DbSpellSetLink* lSpellSetLink : lValue) {
				delete lSpellSetLink;
			}
		}

		_SpellSets.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbSpellSetLink* lSpellSetLink = new DbSpellSetLink(lRow);
			_SpellSets[lSpellSetLink->GetName()].push_back(lSpellSetLink);
		}
	}
public:
	SpellSetContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~SpellSetContext() { Clear(); }

	void SetParameters(std::string aClassKey) {
		_ClassKey = aClassKey;
	}

	std::map<std::string, std::vector<DbSpellSetLink*>>* GetSpellSets() {
		return &_SpellSets;
	}
};


