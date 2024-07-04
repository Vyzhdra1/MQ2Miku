#pragma once
#include <mq/Plugin.h>
#include "Utils.h"
#include "SettingManager.h"

class TimerManager
{
private:
	inline static TimerManager* _Manager = 0;
	std::map<int, std::map<int, unsigned long>> _Timers;
	unsigned long _ComparisonTime;
public:
	void AddTimer(int aTargetID, int aSpell, unsigned long aDuration) {
		if (aDuration <= SettingManager::Get()->GetUpdateDelay()) return;

		_Timers[aTargetID][aSpell] = _ComparisonTime + aDuration;
	}

	void ClearTimes() {
		_Timers.clear();
	}

	void ClearTargetTimer(int aTargetID) {
		_Timers[aTargetID].clear();
	}

	bool IsTimerDue(int aTargetID, int aSpell) {
		if (!_Timers.count(aTargetID)) return true;
		if (!_Timers[aTargetID].count(aSpell)) return true;
		return _ComparisonTime < _Timers[aTargetID][aSpell];
	}

	void UpdateComparisonTime() {
		_ComparisonTime = Utils::GetClockTime();
	}

	static void Deinit() {
		if (_Manager) {
			delete _Manager;
			_Manager = 0;
		}
	}

	static TimerManager* Get() {
		if (!_Manager) {
			_Manager = new TimerManager();
		}
		return _Manager;
	}
};