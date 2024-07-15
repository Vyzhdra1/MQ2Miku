#pragma once
#include <mq/Plugin.h>
#include"StateManager.h"

class MQ2MikuType : public MQ2Type {
private:
	char _szBuffer[MAX_STRING] = { 0 };
public:
	enum Members {
		States,
		Status,
		Rows,
		Result,
		ResultCode,
		Clear
	};

	MQ2MikuType() : MQ2Type("Miku") {
		TypeMember(Status);
		AddMember(States, "states");

		TypeMember(Rows);
		AddMember(Rows, "rows");

		TypeMember(Result);
		AddMember(Result, "result");

		TypeMember(ResultCode);
		AddMember(ResultCode, "Resultcode");
		AddMember(ResultCode, "resultCode");
		AddMember(ResultCode, "resultcode");
	}

	virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest) override {
		_szBuffer[0] = '\0';
		// Query / Row / Column
		const std::vector<std::string> vArgs;// = KnightlyCommon::String::GetArgsFromString(Index);
		const int argQueryName = 0;
		const int argRow = 1;
		const int argColumn = 2;

		MQTypeMember* pMember = MQ2MikuType::FindMember(Member);
		if (!pMember) {
			return false;
		}

		switch (pMember->ID) {
		case States: {
			Dest.Type = mq::datatypes::pStringType;
			std::string lStateNames = StateManager::Get()->ActiveStateNames();
			// If we have a status set ...
			if (!lStateNames.empty())
			{
				strcpy_s(_szBuffer, lStateNames.c_str());
				Dest.Ptr = &_szBuffer[0];
			}
			else {
				// If we don't have a status, just return null...
				strcpy_s(_szBuffer, "NULL");
				Dest.Ptr = &_szBuffer[0];
			}
			return true;
		}
		default:
			break;
		}
		return false;
	}
};
MQ2MikuType* pSQLiteType = nullptr;

bool SQLiteData(const char* szIndex, MQTypeVar& Dest)
{
	Dest.DWord = 1;
	Dest.Type = pSQLiteType;
	return true;
}