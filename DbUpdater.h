#pragma once
#include "DbManager.h"
#include "DbLoader.h"
#include "DbCharacter.h"
class DbUpdater
{
private:
	const int SHORT_PULSE = 50000;
	const int LONG_PULSE = 1000 * 60 * 5;
	unsigned long _NextShortPulse = 0;
public:
	DbUpdater() {
		OnInit();
		_NextShortPulse = Utils::GetClockTime() + SHORT_PULSE;
	}

	~DbUpdater() {
		//OnDeinit();
	}

	void OnPulse() {
		unsigned long lClockTime = Utils::GetClockTime();

		if (lClockTime >= _NextShortPulse) {
			_NextShortPulse = lClockTime + SHORT_PULSE;
			UpdateCharacter();
		}
	}

	void OnInit() {
		DbLoader::Load();
	}

	void UpdateCharacter() {
		DbManager* lDbManager = DbManager::Get();
		DbCharacter* lCharacter = lDbManager->GetCharacter();

		lCharacter->UpdateLatestData(
			GetPcProfile()->Level,
			GetPcProfile()->Exp,
			GetPcProfile()->AAPointsAssigned[0],
			pZoneInfo->LongName);

		lDbManager->Save(lCharacter);
	}
};