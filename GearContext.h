#pragma once
#include <vector>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbGear.h"
#include "StringParameter.h"

class GearContext : public ContextBase
{
private:
	std::vector<DbGear*> _Gear;
	int _CharacterID = -1;
protected:
	std::string GetQueryString() override {
		return
			"";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		//lResult.push_back(new StringParameter(_ClassKey));
		return lResult;
	}

	void Clear() override {
		for (DbGear* lGear : _Gear) {
			delete lGear;
		}

		_Gear.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			//DbSpell* lSpell = new DbSpell(lRow);
			//_Spells[lSpell->GetSpellKey()] = lSpell;
		}
	}
public:
	GearContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~GearContext() { Clear(); }

	void SetParameters(int aCharacterID) {
		_CharacterID = aCharacterID;
	}

	void AddGear(DbGear* aGear) {
		_Gear.push_back(aGear);
	}

	void SaveAll() {
		std::vector<Query*> lQueries;

		for (DbGear* lGear : _Gear) {
			Query * lQuery = new Query();
			lQuery->SetSql(lGear->GetInsertSql());
			lQuery->AddParameters(lGear->GetInsertParams());
			lQuery->SetMode(QueryMode::WRITE);
			lQueries.push_back(lQuery);
		}

		_Connection->BulkQueryDatabasePrepare(&lQueries);

		for (Query* lQuery : lQueries) {
			delete lQuery;
		}
	}

	void DeleteAll() {
		Clear();

		Query lQuery;

		lQuery.SetSql("DELETE FROM Gear WHERE CharacterID = ? ");
		lQuery.AddParameter(new IntParameter(_CharacterID));
		lQuery.SetMode(QueryMode::WRITE);

		_Connection->QueryDatabasePrepare(&lQuery);
	}
};


