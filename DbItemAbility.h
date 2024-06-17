#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"

class DbItemAbility : public StoredObject
{
private:
	std::string _ItemKey = "";
	std::string _ClassKey = "";
	std::string _Description = "";
	int _ItemID = -1;
	int _AlternateItemID = -1;
	int _SPA = -1;
	int _ItemSlot = -1;
	int _RecastType = -1;
	bool _IsSummoned = false;
	bool _IsBuff = false;
public:
	DbItemAbility() : StoredObject() { }

	DbItemAbility(std::string aClassKey) : StoredObject() {
		_ClassKey = aClassKey;

		_Status = StoredStatus::INSERTED;
	}

	DbItemAbility(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_ItemKey = aRow.GetValue("ItemKey").AsString();
		_Description = aRow.GetValue("Description").AsString();

		_ItemID = aRow.GetValue("ItemID").AsInt();
		_AlternateItemID = aRow.GetValue("AlternateItemID").AsInt();
		_SPA = aRow.GetValue("SPA").AsInt();
		_ItemSlot = aRow.GetValue("ItemSlot").AsInt();
		_RecastType = aRow.GetValue("RecastType").AsInt();

		_IsSummoned = aRow.GetValue("IsSummoned").AsBool();
		_IsBuff = aRow.GetValue("IsBuff").AsBool();

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetItemKey() {
		return _ItemKey;
	}

	std::string GetClassKey() {
		return _ClassKey;
	}

	std::string GetDescription() {
		return _Description;
	}

	int GetItemID() {
		return _ItemID;
	}

	int GetAlternateItemID() {
		return _AlternateItemID;
	}

	int GetSPA() {
		return _SPA;
	}

	int GetItemSlot() {
		return _ItemSlot;
	}

	int GetRecastType() {
		return _RecastType;
	}

	bool GetIsSummoned() {
		return _IsSummoned;
	}

	bool GetIsBuff() {
		return _IsBuff;
	}
};