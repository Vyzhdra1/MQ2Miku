#pragma once
#include <string>
#include <vector>
#include "Token.h"
#ifndef H_Entry
#define H_Entry

class Entry
{
private:
	const char* MOD_MATCH = "modmatch";
	const char* ID_MATCH = "idmatch";
	const char* PARTIAL_MATCH = "partial";
	const char* EXACT_MATCH = "exact";

	std::string _TokenType;
	std::string _Key;
	std::string _ID;
	MatchType _MatchType = MatchType::MATCHUNKNOWN;
	std::string _LevelMod;
	std::string _DemandSet;
	SpawnType _Target;
	int _TargetID = -1;
	int _Timer = -1;
	int _CastLimit = -1;
	bool _IsForced = false;
	bool _Simultaneous = false;
	bool _LoadTrigger = false;
	bool _AllowEmpty = false;

	std::set<Token*> _Conditions;
public:
	~Entry() {
		std::set<Token*>::iterator lIterator = _Conditions.begin();
		while (lIterator != _Conditions.end()) {
			delete *lIterator;
			lIterator++;
		}

		_Conditions.clear();
	}

	std::string GetType() {
		return _TokenType;
	}

	std::string GetDemandSet() {
		return _DemandSet;
	}

	void ClearType() {
		_TokenType = "";
	}

	bool GetSimultaneous() {
		return _Simultaneous;
	}

	bool GetAllowEmpty() {
		return _AllowEmpty;
	}
	
	std::string GetID() {
		return _ID;
	}

	std::string GetKey() {
		return _Key;
	}

	MatchType GetMatchType() {
		return _MatchType;
	}

	std::string GetLevelMod() {
		return _LevelMod;
	}

	SpawnType GetTarget() {
		return _Target;
	}

	int GetTargetID() {
		return _TargetID;
	}

	int GetCastLimit() {
		return _CastLimit;
	}

	int GetTimer() {
		return _Timer;
	}

	bool GetLoadTrigger() {
		return _LoadTrigger;
	}

	std::set<Token*> GetConditions() {
		return _Conditions;
	}

	bool IsForced() {
		return _IsForced;
	}

	void SetMatchType(std::string aMatchType) {
		if (!aMatchType.compare("")) return;

		if (!aMatchType.compare(MOD_MATCH)) {
			_MatchType = MatchType::MODMATCH;
		}
		if (!aMatchType.compare(ID_MATCH)) {
			_MatchType = MatchType::IDMATCH;
		}
		if (!aMatchType.compare(PARTIAL_MATCH)) {
			_MatchType = MatchType::PARTIALMATCH;
		}
		if (!aMatchType.compare(EXACT_MATCH)) {
			_MatchType = MatchType::EXACTMATCH;
		}
	}

	bool ParseAsString(std::string aInput) {
		return Parse(aInput.data());
	}

	bool Parse(char* aInput) {
		const char* KEY = "key";
		const char* ID = "id";
		const char* TYPE = "type";
		const char* MATCHTYPE = "matchtype";
		const char* LEVEL_MOD = "levelmod";
		const char* FORCED = "forced";
		const char* TARGET = "target";
		const char* SIMULTANEOUS = "simultaneous";
		const char* TRIGGERS = "loadtrigger";
		const char* DEMANDSET = "demandset";
		const char* TARGETID = "targetid";
		const char* ALLOWEMPTY = "allowempty";
		const char* TIMER = "timer";
		const char* CASTLIMIT = "castlimit";

		char* lParsePtr;
		char* lNameDelimiter = "|";
		char* lValueDelimiter = ";";
		char* lKey = strtok_s(aInput, lNameDelimiter, &lParsePtr);

		try {
			while (lKey) {
				char* lValue = strtok_s(NULL, lValueDelimiter, &lParsePtr);

				if (lKey != 0 && lValue != 0) {
					if (!_strcmpi(KEY, lKey)) {
						_Key = lValue;
					}
					else if (!_strcmpi(ID, lKey)) {
						_ID = lValue;
					}
					else if (!_strcmpi(TYPE, lKey)) {
						_TokenType = lValue;
					}
					else if (!_strcmpi(MATCHTYPE, lKey)) {
						SetMatchType(lValue);
					}
					else if (!_strcmpi(LEVEL_MOD, lKey)) {
						_LevelMod = lValue;
					}
					else if (!_strcmpi(FORCED, lKey)) {
						_IsForced = !TRUE_STR.compare(lValue);
					}
					else if (!_strcmpi(TARGET, lKey)) {
						_Target = PlayerUtils::StringToSpawnType(lValue);
					}
					else if (!_strcmpi(SIMULTANEOUS, lKey)) {
						_Simultaneous = !TRUE_STR.compare(lValue);
					}
					else if (!_strcmpi(TRIGGERS, lKey)) {
						_LoadTrigger = !TRUE_STR.compare(lValue);
					}
					else if (!_strcmpi(DEMANDSET, lKey)) {
						_DemandSet = lValue;
					}
					else if (!_strcmpi(TARGETID, lKey)) {
						_TargetID = Utils::StrToInt(lValue, -1);
					}
					else if (!_strcmpi(ALLOWEMPTY, lKey)) {
						_AllowEmpty = true;
					}
					else if (!_strcmpi(TIMER, lKey)) {
						_Timer = Utils::StrToInt(lValue, -1);
					}
					else if (!_strcmpi(CASTLIMIT, lKey)) {
						_CastLimit = Utils::StrToInt(lValue, 1);
					}
					else {
						Token* lToken = new Token(lKey, lValue);

						_Conditions.insert(lToken);
					}
				}

				lKey = strtok_s(NULL, lNameDelimiter, &lParsePtr);
			}
			return true;
		} catch (int e) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to parse line: ", lParsePtr);
			return false;
		}
	}
};
#endif