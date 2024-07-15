#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbGuildMember.h"

class GuildContext : public ContextBase
{
private:
	std::vector<DbGuildMember*> _Members;
	std::map<std::string, std::vector<DbGuildMember*>> _CharacterMap;
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		Character.CharacterID, \
			 		Character.Name, \
			 		Character.ClassKey, \
					coalesce(Settings.Value, CharacterClass.DefaultRole) AS Role \
			 	FROM  \
			 	    Character  \
				INNER JOIN CharacterClass ON \
					CharacterClass.ClassKey = Character.ClassKey \
				LEFT OUTER JOIN SettingsOverride ON \
					Settings.Name = 'role' \
					AND SettingsOverride.CharacterID = Character.CharacterID \
				WHERE \
					Character.IsValid ";
	}

	void Clear() override {
		for (DbGuildMember* lCharacter : _Members) {
			delete lCharacter;
		}

		_Members.clear();
		_CharacterMap.clear();
	}

	void AddCharacterToMap(std::string aKey, DbGuildMember * aValue) {
		std::vector<DbGuildMember*> lMembers;
		if (_CharacterMap.count(aKey) == 0) {
			_CharacterMap[aKey] = lMembers;
		}
		_CharacterMap[aKey].push_back(aValue);
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbGuildMember* lCharacter = new DbGuildMember(lRow);
			_Members.push_back(lCharacter);
			AddCharacterToMap(lCharacter->Name(), lCharacter);
			AddCharacterToMap(lCharacter->Role(), lCharacter);
			AddCharacterToMap(lCharacter->ClassKey(), lCharacter);
		}
	}
public:
	GuildContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~GuildContext() { Clear(); }

	std::vector<DbGuildMember*>* GetCharacters() {
		return &_Members;
	}

	std::vector<DbGuildMember*>* GetCharacters(std::string aKey) {
		return &_CharacterMap[aKey];
	}
};