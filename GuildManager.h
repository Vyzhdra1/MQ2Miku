#pragma once
#include <mq/Plugin.h>
#include "Utils.h"
#include "DbGroup.h"
#include "DbManager.h"

class GuildManager
{
private:
	inline static GuildManager* _Manager = 0;

	int GetlGroupID(std::string aGroupName) {
		std::vector<DbGroup*> lGroupData = *DbManager::Get()->GetGroupContext()->GetGroups();
		for (DbGroup* lGroup : lGroupData) {
			if (lGroup->GroupKey().compare(aGroupName)) continue;
			if (lGroup->Name().compare(GetCharInfo()->Name)) continue;

			return lGroup->GroupID();
		}
		return -1;
	}

	std::vector<DbGroup*> GetMatches(std::string aGroupName, int aGroupID) {
		std::vector<DbGroup*> lResult;
		std::vector<DbGroup*> lGroupData = *DbManager::Get()->GetGroupContext()->GetGroups();
		for (DbGroup* lGroup : lGroupData) {
			if (lGroup->GroupKey().compare(aGroupName)) continue;
			if (lGroup->GroupID() != aGroupID) continue;
			if (!lGroup->Name().compare(GetCharInfo()->Name)) continue;

			lResult.push_back(lGroup);
		}
		return lResult;
	}
public:
	void Invite(std::string aGroupName) {
		int lGroupID = GetlGroupID(aGroupName);

		if (lGroupID == -1) return;

		std::vector<DbGroup*> lMatches = GetMatches(aGroupName, lGroupID);

		for (DbGroup* lGroup : lMatches) {
			Utils::MikuSendCommand("/invite " + lGroup->Name());
		}
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static GuildManager* Get() {
		if (!_Manager) {
			_Manager = new GuildManager();
		}
		return _Manager;
	}
};