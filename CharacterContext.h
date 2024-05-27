#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbCharacter.h"

class CharacterContext : public ContextBase
{
private:
	DbCharacter * _Character = 0;
	std::string _Name;
	std::string _Server;
	std::string _ClassKey;
protected:
	std::string GetQueryString() override {
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

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		lResult.push_back(new StringParameter(_Name));
		lResult.push_back(new StringParameter(_Server));
		lResult.push_back(new StringParameter(_ClassKey));

		return lResult;
	}

	void Clear() override {
		if (_Character) {
			delete _Character;
		}
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			_Character = new DbCharacter(lRow);
		}
	}
public:
	CharacterContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~CharacterContext() { Clear(); }

	void SetParameters(std::string aName, std::string aServer, std::string aClassKey) {
		_Name = aName;
		_Server = aServer;
		_ClassKey = aClassKey;
	}

	DbCharacter * GetCharacter() {
		return _Character;
	}
};




