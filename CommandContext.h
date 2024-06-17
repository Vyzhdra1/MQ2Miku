#pragma once
#include <map>
#include <string>
#include "DbConnection.h"
#include "ContextBase.h"
#include "DbCommand.h"
#include "StringParameter.h"

class CommandContext : public ContextBase
{
private:
	std::map<std::string, DbCommand*> _Commands;
protected:
	std::string GetQueryString() override {
		return
			"	SELECT \
			 		Command.CommandKey, \
			 		Command.CommandText \
			 	FROM  \
			 	    Command ";
	}

	std::vector<ParameterBase*> GetParams() override {
		std::vector<ParameterBase*> lResult;
		return lResult;
	}

	void Clear() override {
		for (const auto& [lKey, lValue] : _Commands) {
			delete lValue;
		}

		_Commands.clear();
	}

	void LoadDbObjects(SqlResult aResult) override {
		for (SqlRow lRow : aResult.GetRows()) {
			DbCommand* lCommand = new DbCommand(lRow);
			_Commands[lCommand->GetCommandKey()] = lCommand;
		}
	}
public:
	CommandContext(DbConnection* aConnection) : ContextBase(aConnection) {}
	~CommandContext() { Clear(); }

	std::map<std::string, DbCommand*>* GetCommands() {
		return &_Commands;
	}
};


