#pragma once
#include <string>

class DbCharacter
{
private:
	int _Id = -1;
	std::string _Name;
	int _Level;
	std::string _Class;
public:
	void SetConnection(std::string aConnection) {

	}

	void InsertOrUpdate() {

	}



	static void Deinit() {
		if (_Workspace) {
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