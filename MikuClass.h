#pragma once
#include "MikuPlayer.h"

class MikuBerserker : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuMonk : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuRogue : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuBeastlord : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = true;
		_Role = DPS_STR;
	}
};

class MikuRanger : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuBard : public MikuPlayer {
private:
	const int MAX_SONG_ITERATION_BEFORE_CANCEL = 1;
	int _StopSongIterationCount = 0;
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = DPS_STR;
	}

	void Pulse() override {
		if (!(pCastingWnd && (CSidlScreenWnd*)pCastingWnd->dShow) && (pCharSpawn && ((PSPAWNINFO)pCharSpawn)->CastingData.SpellID != -1)) {
			_StopSongIterationCount++;
			if (_StopSongIterationCount > MAX_SONG_ITERATION_BEFORE_CANCEL) {
				EzCommand("/stopsong");
				_StopSongIterationCount = 0;
			}
		}

		MikuPlayer::Pulse();
	}
};

class MikuWarrior : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = TANK_STR;
	}
};

class MikuShadowKnight : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = true;
		_Role = TANK_STR;
	}
};

class MikuPaladin : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = TANK_STR;
	}
};

class MikuCleric : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = false;
		_Role = HEALER_STR;
	}
};

class MikuShaman : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = true;
		_IsPetClass = true;
		_Role = HEALER_STR;
	}
};

class MikuDruid : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = false;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuWizard : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = false;
		_IsPetClass = false;
		_Role = DPS_STR;
	}
};

class MikuMage : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = false;
		_IsPetClass = true;
		_Role = DPS_STR;
	}
};

class MikuNecromancer : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = false;
		_IsPetClass = true;
		_Role = DPS_STR;
	}
};

class MikuEnchanter : public MikuPlayer {
protected:
	void InitClass() {
		_IsMelee = false;
		_IsPetClass = true;
		_Role = DPS_STR;
	}
};
