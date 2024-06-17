#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"
#include "ParameterBase.h"
#include "StringParameter.h"
#include "IntParameter.h"
#include "BoolParameter.h"

class DbGear : public StoredObject
{
private:
	int _CharacterID = -1;
	int _SlotID = -1;
	int _ItemID = -1;
	std::string _ItemName = "";
	int _HP = -1;
	int _AC = -1;
	int _RequiredLevel = -1;
	bool _HasPetFocus = false;
	bool _HasHaste = false;
public:
	DbGear() : StoredObject() { }

	DbGear(
		int aCharacterID,
		int aSlotID,
		int aItemID,
		std::string aItemName,
		int aHP,
		int aAC,
		int aRequiredLevel,
		bool aHasPetFocus,
		bool aHasHaste
	) : StoredObject() {
		_CharacterID = aCharacterID;
		_SlotID = aSlotID;
		_ItemID = aItemID;
		_ItemName = aItemName;
		_HP = aHP;
		_AC = aAC;
		_RequiredLevel = aRequiredLevel;
		_HasPetFocus = aHasPetFocus;
		_HasHaste = aHasHaste;

		_Status = StoredStatus::INSERTED;
	}

	DbGear(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_CharacterID = aRow.GetValue("CharacterID").AsInt();
		_SlotID = aRow.GetValue("SlotID").AsInt();
		_ItemID = aRow.GetValue("ItemID").AsInt();
		_HP = aRow.GetValue("HP").AsInt();
		_AC = aRow.GetValue("AC").AsInt();
		_RequiredLevel = aRow.GetValue("RequiredLevel").AsInt();

		_ItemName = aRow.GetValue("ItemName").AsString();

		_HasPetFocus = aRow.GetValue("HasPetFocus").AsString().compare("Y") == 0;
		_HasHaste = aRow.GetValue("HasHaste").AsString().compare("Y") == 0;

		_IsLoaded = true;
		_Status = StoredStatus::NOCHANGE;
	}

	std::string GetInsertSql() override {
		return
			"	INSERT INTO Gear (CharacterID, SlotID, ItemID, ItemName, HP, AC, RequiredLevel, HasPetFocus, HasHaste) \
				VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?) ";
	}

	std::vector<ParameterBase*> GetInsertParams() override {
		std::vector<ParameterBase*> lResult;

		lResult.push_back(new IntParameter(_CharacterID));
		lResult.push_back(new IntParameter(_SlotID));
		lResult.push_back(new IntParameter(_ItemID));
		lResult.push_back(new StringParameter(_ItemName));
		lResult.push_back(new IntParameter(_HP));
		lResult.push_back(new IntParameter(_AC));
		lResult.push_back(new IntParameter(_RequiredLevel));
		lResult.push_back(new BoolParameter(_HasPetFocus));
		lResult.push_back(new BoolParameter(_HasHaste));

		return lResult;
	}
};