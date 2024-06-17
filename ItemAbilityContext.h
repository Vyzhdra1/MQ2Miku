#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbItemAbility.h"
#include "StringParameter.h"

class ItemAbilityContext : public ContextBase
{
private:
	std::map<std::string, DbItemAbility*> _ItemAbilities;
	std::string _ClassKey = "";
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		ItemAbility.ItemKey, \
			 		ItemAbility.ClassKey, \
			 		ItemAbility.ItemID, \
			 		ItemAbility.AlternateItemID, \
					ItemAbility.SPA, \
					ItemAbility.ItemSlot, \
					ItemAbility.RecastType, \
					ItemAbility.IsSummoned, \
					ItemAbility.Description, \
			 		ItemAbility.IsBuff \
			 	FROM  \
			 	    ItemAbility \
				WHERE \
					ItemAbility.ClassKey = ? \
					OR ItemAbility.ClassKey LIKE '%' || ? || '%' ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter("ALL"));
		lResult.push_back(new StringParameter(_ClassKey));
		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _ItemAbilities) {
			delete lValue;
		}

		_ItemAbilities.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbItemAbility* lItemAbility = new DbItemAbility(lRow);
			_ItemAbilities[lItemAbility->GetItemKey()] = lItemAbility;
		}
	}
public:
	ItemAbilityContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~ItemAbilityContext() { Clear(); }

	void SetParameters(std::string aClassKey) {
		_ClassKey = aClassKey;
	}

	std::map<std::string, DbItemAbility*>* GetItemAbilities() {
		return &_ItemAbilities;
	}
};


