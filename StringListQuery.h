#pragma once
#include "DbManager.h"
class StringListQuery
{
private:
	static std::vector<std::string> CreateStringList(Query* aQuery, std::string aField) {
		DbManager::Get()->ExecSql(aQuery);
		SqlResult lSqlResult = aQuery->GetResult();

		std::vector<std::string> lResult;
		for (SqlRow lRow : lSqlResult.GetRows()) {
			lResult.push_back(lRow.GetValue(aField).AsString());
		}
		return lResult;
	}
public:
	static std::vector<std::string> GetTankList() {
		std::string lSQL =
			"	SELECT \
					Character.Name \
				FROM \
					Character \
				WHERE \
					Character.Level >= 115 \
					AND Character.CharacterClass IN (?, ?, ?) \
					AND Character.IsValid = 'Y' ";

		Query lQuery;
		lQuery.SetSql(lSQL);
		lQuery.AddParameter(new StringParameter("SHD"));
		lQuery.AddParameter(new StringParameter("PAL"));
		lQuery.AddParameter(new StringParameter("WAR"));

		return CreateStringList(&lQuery, "Name");
	}
};