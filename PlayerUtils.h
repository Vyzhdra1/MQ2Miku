#pragma once
#include <mq/Plugin.h>

enum SpawnType { UNKNOWN, SELF, TANK, GROUP, LOWEST_HP_GROUP, MYTARGET, DEAD_PLAYER, ATTACK_TARGET, MY_PET, XTARGET };
bool* StickProc;
class PlayerUtils
{
private:
public:
	static bool* _StickProc;

	inline static std::string SELF_STR = "self";
	inline static std::string GROUP_STR = "group";
	inline static std::string LOWEST_STR = "lowest";
	inline static std::string TANK_STR = "tank";
	inline static std::string TANK_TAR_STR = "tanktar";
	inline static std::string MY_TARGET_STR = "mytarget";
	inline static std::string DEAD_STR = "deadplayer";
	inline static std::string ATTACKTAR_STR = "attacktarget";
	inline static std::string MY_PET_STR = "mypet";
	inline static std::string XTARGET_STR = "xtar";

	static SpawnType StringToSpawnType(std::string aValue) {
		if (!SELF_STR.compare(aValue.c_str())) {
			return SpawnType::SELF;
		}
		else if (!TANK_STR.compare(aValue.c_str())) {
			return SpawnType::TANK;
		}
		else if (!GROUP_STR.compare(aValue.c_str())) {
			return SpawnType::GROUP;
		}
		else if (!LOWEST_STR.compare(aValue.c_str())) {
			return SpawnType::LOWEST_HP_GROUP;
		}
		else if (!MY_TARGET_STR.compare(aValue.c_str())) {
			return SpawnType::MYTARGET;
		}
		else if (!DEAD_STR.compare(aValue.c_str())) {
			return SpawnType::DEAD_PLAYER;
		}
		else if (!ATTACKTAR_STR.compare(aValue.c_str())) {
			return SpawnType::ATTACK_TARGET;
		}
		else if (!MY_PET_STR.compare(aValue.c_str())) {
			return SpawnType::MY_PET;
		}
		else if (!XTARGET_STR.compare(aValue.c_str())) {
			return SpawnType::XTARGET;
		}
		return UNKNOWN;
	}

	static int ValuePerc(int64_t aMax, int64_t aCurrent) {
		return aMax == 0 ? 0 : aCurrent * 100 / aMax;
	}

	static int HealthPerc() {
		return ValuePerc(pLocalPlayer ->HPMax, GetCharInfo()->pSpawn->HPCurrent);
	}

	static int EndurancePerc() {
		return ValuePerc(pLocalPlayer->EnduranceMax, GetCharInfo()->pSpawn->EnduranceCurrent);
	}

	static int ManaPerc() {
		return ValuePerc(pLocalPlayer->ManaMax, GetCharInfo()->pSpawn->ManaCurrent);
	}

	static bool InCombat() {
		return ((CPlayerWnd*)pPlayerWnd)->CombatState == eCombatState_Combat && ((CXWnd*)pPlayerWnd)->GetChildItem("PW_CombatStateAnim");
	}

	static bool IsRezer() {
		return
			(GetPcProfile()->Class == Cleric) ||
			(GetPcProfile()->Class == Shaman) ||
			(GetPcProfile()->Class == Druid) ||
			(GetPcProfile()->Class == Paladin);
	}

	static bool IsResting() {
		return ((CPlayerWnd*)pPlayerWnd)->CombatState == 2;
	}

	static bool IsTimerState() {
		return ((CPlayerWnd*)pPlayerWnd)->CombatState == eCombatState_Timer;
	}

	static bool Invisible() {
		return pLocalPlayer->HideMode != 0;
	}

	static bool IsAutoOn() {
		return pEverQuestInfo->bAutoAttack;
	}

	static bool CanExecuteAbility() {
		return 
			(pLocalPlayer->StandState != STANDSTATE_FEIGN) &&
			(pLocalPlayer->StandState != STANDSTATE_DEAD) &&
			(pLocalPlayer->StandState != STANDSTATE_DUCK);
	}

	static bool CanAssist(PSPAWNINFO pChar, char * aTargetID, int aRange) {
		return
			Utils::IsTargetInRange(pChar, aTargetID, aRange) &&
			(pLocalPlayer->StandState != STANDSTATE_FEIGN) &&
			(pLocalPlayer->StandState != STANDSTATE_DEAD) &&
			(pLocalPlayer->StandState != STANDSTATE_DUCK);
	}

	static bool IsSitting() {
		return (pLocalPlayer->StandState == STANDSTATE_SIT);
	}

	static bool IsStanding() {
		return (pLocalPlayer->StandState == STANDSTATE_STAND);
	}

	static bool IsSticking() {
		return *StickProc;
	}

	static bool HasPet() {
		return pLocalPlayer->PetID != -1;
	}

	static int GetBuffTimer(PcProfile * aSpawn, long aSpellID) {
		for (unsigned long nBuff = 0; nBuff < NUM_LONG_BUFFS; nBuff++)
		{
			if (aSpawn->GetEffect(nBuff).SpellID == aSpellID)
			{
				return aSpawn->GetEffect(nBuff).Duration;
			}
		}

		for (unsigned long nBuff = 0; nBuff < NUM_SHORT_BUFFS; nBuff++)
		{
			if (aSpawn->GetTempEffect(nBuff).SpellID == aSpellID)
			{
				return aSpawn->GetTempEffect(nBuff).Duration;
			}
		}

		return 0;
	}

	static int GetClosestXTarget() {
		ExtendedTargetList* lTargetList = GetCharInfo()->pExtendedTargetList;

		if (!lTargetList) return 0;

		int lSpawn = 0;
		int lDistance = 60;

		for (ExtendedTargetSlot* lIterator = lTargetList->begin(); lIterator != lTargetList->end(); lIterator++) {
			if (!lIterator) continue;

			if (!lIterator->SpawnID) continue;

			PlayerClient* lTarget = GetSpawnByID(lIterator->SpawnID);

			if (!lTarget) continue;

			if (!GetCharInfo()->pSpawn) continue;

			int lNewDistance = (int) DistanceToSpawn(GetCharInfo()->pSpawn, lTarget);
			int lTargetOfTargetID = lTarget->TargetOfTarget;

			PlayerClient* lTargetOfTarget = 0;

			if (lTargetOfTargetID) {
				lTargetOfTarget = GetSpawnByID(lTargetOfTargetID);
			}

			if ((lTargetOfTarget) && (lTargetOfTargetID != GetCharInfo()->pSpawn->SpawnID)) {
				lNewDistance = (int)DistanceToSpawn(lTarget, lTargetOfTarget);
				if (lNewDistance < 60) {
					return lIterator->SpawnID;
				}
			}

			if (lNewDistance < lDistance) {
				lDistance = lNewDistance;
				lSpawn = lIterator->SpawnID;
			}
		}

		return lSpawn;
	}
};
bool* PlayerUtils::_StickProc = 0;