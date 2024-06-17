#pragma once
#include <string>
#include <mq/Plugin.h>
#include "sqlite3.h"

#include <fstream>
#include <filesystem>
#include "Utils.h"
#include "ParameterBase.h"
#include "Query.h"

class DbConnection
{
private:
	sqlite3* _Connection = 0;
	inline static std::vector<std::map<std::string, std::string>> _Result;

	enum custom_sqlite_result
	{
		QUERY_ACTIVE = -1,
		QUERY_NOT_FOUND = -2,
		CONVERSION_ERROR = -3,
		MAP_NOT_ERASED = -10,
		CONN_NOT_FOUND = -11,
		UNKNOWN_ERROR = -12,
		CONN_EXISTS = -13,
	};

	std::string GetDatabasePath() {
		char lDbLocation[2048] = { 0 };
		sprintf_s(lDbLocation, "%s\\config\\MQ2Miku\\MikuDB.db", gPathMQRoot);

		return std::string(lDbLocation);
	}

	std::filesystem::path MakeAbsolutePath(std::filesystem::path pathFile) {
		// If the path is relative, change the directory it's relative TO.
		if (pathFile.is_relative()) {
			// Tack on the base MQ2 directory
			pathFile = gPathMQRoot / pathFile;
		}
		return pathFile;
	}

	bool IsValidFilePath(std::filesystem::path pathFile) {
		bool bReturn = false;
		// Make this an absolute path
		pathFile = MakeAbsolutePath(pathFile);
		std::error_code ec_exists;
		// Check if the file already exists (if it does, we think it's valid)
		if (!std::filesystem::exists(pathFile, ec_exists)) {
			// If the file doesn't already exist then it doesn't necessarily mean it can't be created
			std::ofstream writePath(pathFile);
			if (writePath) {
				// Close any open write handles
				writePath.close();
				bReturn = true;
			}
		}
		// The file exists already, which means it's valid
		else {
			bReturn = true;
		}
		return bReturn;
	}

	void Close() {
		if (_Connection) {
			sqlite3_close_v2(_Connection);
		}
	}

	static int CallbackSQLite(void* data, int aColumnCount, char** aValueArray, char** aColumnNameArray) {
		std::map<std::string, std::string> lRow;

		for (int i = 0; i < aColumnCount; i++) {
			lRow[aColumnNameArray[i]] = aValueArray[i] ? aValueArray[i] : "";
		}

		_Result.push_back(lRow);

		return 0;
	}
public:
	~DbConnection() {
		Close();
	}

	void OpenDatabase() {
		int iResult = custom_sqlite_result::UNKNOWN_ERROR;
		int iOpenFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		std::filesystem::path lPath = GetDatabasePath();

		// Sanitize the path if we need to, otherwise take it however the sender passed it
		if (IsValidFilePath(lPath)) {
			lPath = MakeAbsolutePath(lPath);
		}

		iResult = CONN_EXISTS;
		if (!_Connection) {
			iResult = sqlite3_open_v2(lPath.string().c_str(), &_Connection, iOpenFlags, nullptr);
		}

		if (iResult != SQLITE_OK) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to connect to database: ", GetDatabasePath());
			return;
		}
		sqlite3_busy_timeout(_Connection, 10000);
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Successfully connected to database: ", GetDatabasePath());
	}

	bool IsSuccessfulSqlResult(int aSqlResult) {
		if (aSqlResult == SQLITE_OK) {
			return true;
		}

		Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to execute query: ", sqlite3_errmsg(_Connection));

		return false;
	}

	void BeginTransaction(QueryMode aMode) {
		if (aMode == QueryMode::WRITE) {
			sqlite3_exec(_Connection, "BEGIN IMMEDIATE TRANSACTION", 0, 0, 0);
		}
	}

	void FinaliseTransaction(QueryMode aMode, sqlite3_stmt* aStatement) {
		IsSuccessfulSqlResult(sqlite3_finalize(aStatement));

		if (aMode == QueryMode::WRITE) {
			sqlite3_exec(_Connection, "COMMIT", 0, 0, 0);
		}
	}

	void BulkQueryDatabasePrepare(
		std::vector<Query*> * aQueries
	) {
		if (!_Connection) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Connection not found");
			return;
		}

		QueryMode lMode = QueryMode::READ;

		sqlite3_stmt* lStatement = 0;
		int iReturn = -1;
		bool lStatementInitialised = false;
		for (Query* lQuery : *aQueries) {
			if (!lStatementInitialised) {
				lMode = lQuery->GetMode();
				BeginTransaction(lMode);
				iReturn = sqlite3_prepare_v2(_Connection, lQuery->GetSql().c_str(), -1, &lStatement, NULL);
				lStatementInitialised = true;

				if (!IsSuccessfulSqlResult(iReturn)) {
					FinaliseTransaction(lMode, lStatement);
					return;
				}
			}

			int lIndex = 0;
			for (ParameterBase* lParam : lQuery->GetParams()) {
				lIndex++;

				if (!IsSuccessfulSqlResult(lParam->Bind(lStatement, lIndex))) {
					FinaliseTransaction(lMode, lStatement);
					return;
				}
			}

			while (iReturn = sqlite3_step(lStatement) == SQLITE_ROW) {
				lQuery->AddResultRow(lStatement);
			}

			sqlite3_reset(lStatement);
		}

		FinaliseTransaction(lMode, lStatement);
	}

	void QueryDatabasePrepare(
		Query * aQuery
	) {
		if (!_Connection) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Connection not found");
			return;
		}

		BeginTransaction(aQuery->GetMode());

		sqlite3_stmt* lStatement;
		int iReturn = sqlite3_prepare_v2(_Connection, aQuery->GetSql().c_str(), -1, &lStatement, NULL);

		if (!IsSuccessfulSqlResult(iReturn)) return;

		int lIndex = 0;
		for (ParameterBase * lParam : aQuery->GetParams()) {
			lIndex++;

			if (!IsSuccessfulSqlResult(lParam->Bind(lStatement, lIndex))) {
				FinaliseTransaction(aQuery->GetMode(), lStatement);
				return;
			}
		}

		while (iReturn = sqlite3_step(lStatement) == SQLITE_ROW) {
			aQuery->AddResultRow(lStatement);
		}

		FinaliseTransaction(aQuery->GetMode(), lStatement);
	}

	std::vector<std::map<std::string, std::string>> QueryDatabase(
		std::string& aQueryName, 
		const std::string& aQueryStatement
	) {
		_Result.clear();

		int iReturn = UNKNOWN_ERROR;
		char* zErrMsg = nullptr;
		if (!_Connection) {
			iReturn = CONN_NOT_FOUND;
		}
		else {
			iReturn = sqlite3_exec(_Connection, aQueryStatement.c_str(), CallbackSQLite, &aQueryName, &zErrMsg);
			if (iReturn != SQLITE_OK) {
				if (zErrMsg == nullptr) {
					Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to execute query: ", aQueryName);
				}
				else {
					Utils::MikuEcho(Utils::FAIL_COLOR, "Failed to execute query: ", std::string(zErrMsg));
					sqlite3_free(zErrMsg);
				}
			}
			else {
				Utils::MikuEcho(Utils::SUCCESS_COLOR, "ExecutedQuery: ", aQueryName);
			}
		}
		return _Result;
	}
};