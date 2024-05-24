#pragma once
#include <string>
#include <mq/Plugin.h>
#include "PlayerUtils.h"

class PlayerBuff {
private:
	std::string _BuffName;
	bool _IsRequested;
	bool _LastRequestTime;
public:
	PlayerBuff(std::string aBuffName) { 
		_BuffName = aBuffName;
		_IsRequested = false;
	}

	bool HasBuff() {
		return false;
	}

	void Request() {

	}
};