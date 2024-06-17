#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbSpell.h"
#include "StringParameter.h"

class SpellsContext : public ContextBase
{
private:
	std::map<std::string, DbSpell*> _Spells;
	std::string _ClassKey = "";
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		Spells.SpellKey, \
			 		Spells.ClassKey, \
			 		Spells.Description, \
			 		Spells.LevelOffset, \
					Spells.ExactLevel, \
			 		Spells.SPA1, \
			 		Spells.SPA2, \
					Spells.SPA3, \
			 		Spells.Timer, \
					Spells.Target, \
					Spells.IsCombatAbility, \
			 		Spells.HasRecourse, \
			 		Spells.IsBuff \
			 	FROM  \
			 	    Spells \
				WHERE \
					Spells.ClassKey = ? ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_ClassKey));
		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _Spells) {
			delete lValue;
		}

		_Spells.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbSpell* lSpell = new DbSpell(lRow);
			_Spells[lSpell->GetSpellKey()] = lSpell;
		}
	}
public:
	SpellsContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~SpellsContext() { Clear(); }

	void SetParameters(std::string aClassKey) {
		_ClassKey = aClassKey;
	}

	std::map<std::string, DbSpell*>* GetSpells() {
		return &_Spells;
	}
};


