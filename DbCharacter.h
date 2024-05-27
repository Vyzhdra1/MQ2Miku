#pragma once
#include <string>
#include "SqlRow.h"
#include "StoredObject.h"
#include "ParameterBase.h"
#include "StringParameter.h"
#include "IntParameter.h"

class DbCharacter: public StoredObject
{
private:
	int _CharacterId = -1;
	std::string _Name = "";
	int _Level = -1;
	std::string _Server = "";
	std::string _ClassKey = "";
	std::string _AccountName = "";
	std::string _DefaultRole = "";
	bool _IsValid = true;
public:
	DbCharacter() : StoredObject() { }

	DbCharacter (
		std::string aName,
		int aLevel,
		std::string aServer,
		std::string aCharacterClass,
		std::string aAccountName) : StoredObject() {
		_Name = aName;
		_Level = aLevel;
		_Server = aServer;
		_ClassKey = aCharacterClass;
		_AccountName = aAccountName;

		_Status = StoredStatus::INSERTED;
	}

	DbCharacter(SqlRow aRow) : StoredObject() {
		LoadFromDb(aRow);
		_IsLoaded = true;
	}

	void LoadFromDb(SqlRow aRow) override {
		_CharacterId = aRow.GetValue("CharacterID").AsInt();
		_Name = aRow.GetValue("Name").AsString();
		_Level = aRow.GetValue("Level").AsInt();
		_Server = aRow.GetValue("Server").AsString();
		_ClassKey = aRow.GetValue("ClassKey").AsString();
		_AccountName = aRow.GetValue("AccountName").AsString();
		_DefaultRole = aRow.GetValue("DefaultRole").AsString();
		_IsValid = aRow.GetValue("IsValid").AsString().compare("Y") == 0;
		_IsLoaded = true;
	}

	const int GetCharacterID() {
		return _CharacterId;
	}

	std::string GetDefaultRole() {
		return _DefaultRole;
	}

	std::string GetInsertSql() override {
		return
			"	INSERT INTO Character (Name, Level, Server, ClassKey, AccountName, IsValid) \
				VALUES (?, ?, ?, ?, ?, ?) ";
	}

	std::string GetUpdateSql() override {
		return
			"	UPDATE Character \
			 	SET \
					Level = ?, \
					ClassKey = ? \
				WHERE \
					CharacterID = ? ";
	}

	std::string GetFetchSql() {
		return
			"	SELECT \
			 		Character.CharacterID, \
			 		Character.Name, \
			 		Character.ClassKey, \
			 		Character.Server, \
			 		Character.Level, \
			 		Character.IsValid, \
			 		Character.AccountName, \
					CharacterClass.DefaultRole \
			 	FROM  \
			 	    Character  \
				INNER JOIN CharacterClass ON \
					CharacterClass.ClassKey = Character.ClassKey \
				WHERE \
					Character.Name = ? \
					AND Character.Server = ? \
					AND Character.ClassKey = ? ";
	}

	std::vector<ParameterBase*> GetInsertParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_Name));
		lResult.push_back(new IntParameter(_Level));
		lResult.push_back(new StringParameter(_Server));
		lResult.push_back(new StringParameter(_ClassKey));
		lResult.push_back(new StringParameter(_AccountName));
		lResult.push_back(new StringParameter("Y"));

		return lResult;
	}

	std::vector<ParameterBase*> GetUpdateParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new IntParameter(_Level));
		lResult.push_back(new StringParameter(_ClassKey));
		lResult.push_back(new IntParameter(_CharacterId));

		return lResult;
	}

	std::vector<ParameterBase*> GetFetchParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_Name));
		lResult.push_back(new StringParameter(_Server));
		lResult.push_back(new StringParameter(_ClassKey));

		return lResult;
	}
};