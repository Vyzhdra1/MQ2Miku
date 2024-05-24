#pragma once
#include <string>
#include <mq/Plugin.h>
#include "sqlite3.h"

#include <fstream>
#include <filesystem>
#include "Utils.h"

class GlobalWorkspace
{
private:
	inline static GlobalWorkspace* _Workspace = 0;
	sqlite3* _Connection = 0;

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

public:
	std::string GetDatabasePath() {
		char lDbLocation[2048] = { 0 };
		sprintf_s(lDbLocation, "%s\\config\\MQ2Miku\\Miku.ini", gPathMQRoot);

		return std::string(lDbLocation);
	}

	static std::filesystem::path MakeAbsolutePath(std::filesystem::path pathFile) {
		// If the path is relative, change the directory it's relative TO.
		if (pathFile.is_relative()) {
			// Tack on the base MQ2 directory
			pathFile = gPathMQRoot / pathFile;
		}
		return pathFile;
	}

	static bool IsValidFilePath(std::filesystem::path pathFile) {
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
		}
	}

	void QueryDatabase(int (*aCallback)(void*, int, char**, char**), std::string aQueryName, const std::string& aQueryStatement) {
		int iReturn = UNKNOWN_ERROR;
		char* zErrMsg = nullptr;
		if (!_Connection) {
			iReturn = CONN_NOT_FOUND;
		}
		else {
			iReturn = sqlite3_exec(_Connection, aQueryStatement.c_str(), aCallback, &aQueryName, &zErrMsg);
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
	}

	void Close() {
		if (_Connection) {
			sqlite3_close_v2(_Connection);
		}
	}

	static void Deinit() {
		if (_Workspace) {
			_Workspace->Close();
			delete _Workspace;
			_Workspace = 0;
		}
	}

	static GlobalWorkspace* Get() {
		if (!_Workspace) {
			_Workspace = new GlobalWorkspace();
		}
		return _Workspace;
	}
};