#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbGroup.h"

class GroupContext : public ContextBase
{
private:
	std::vector<DbGroup*> _Groups;
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		Group.*, \
			 		Character.Name, \
			 	FROM  \
			 	    Group  \
				INNER JOIN Character ON \
					Character.CharacterID = Group.CharacterID ";
	}

	void Clear() override {
		for (DbGroup* lGroup : _Groups) {
			delete lGroup;
		}

		_Groups.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbGroup* lGroup = new DbGroup(lRow);
			_Groups.push_back(lGroup);
		}
	}
public:
	GroupContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~GroupContext() { Clear(); }

	std::vector<DbGroup*>* GetGroups() {
		return &_Groups;
	}
};