#ifndef UTILS
#define UTILS
#include <sstream>
#include <string>

class Utils
{
public:
	static const char WARNING_COLOR = 'y';
	static const char FAIL_COLOR = 'r';
	static const char NEUTRAL_COLOR = 'p';
	static const char SUCCESS_COLOR = 'g';
	static const char BLUE_COLOR = 'u';
	static const char MAGENTA_COLOR = 'm';

	static int StrToInt(const char* aValue, int aDefault)
	{
		try {
			return atoi(aValue);
		}
		catch (int x) {
			return aDefault;
		}
	}


	static bool IsZoneShortNameMatch(std::string aZoneShortName) {
		if (!pZoneInfo) return false;

		return !aZoneShortName.compare(pZoneInfo->ShortName);
	}

	static bool IsTargetInRange(PSPAWNINFO pChar, char * aTargetID, int aRange) {
		DWORD lTargetID = Utils::StrToInt(aTargetID, -1);

		if (lTargetID == -1) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid Target ID: ", aTargetID);
			return false;
		}

		return IsTargetInRange(pChar, lTargetID, aRange);
	}

	static bool IsTargetInRange(PSPAWNINFO pChar, DWORD aTargetID, int aRange) {
		PSPAWNINFO lTarget = (PSPAWNINFO)GetSpawnByID(aTargetID);

		if (!lTarget) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Target not found: ", aTargetID);
			return false;
		}
		else if (DistanceToSpawn(pChar, lTarget) > aRange) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Target not in range: ", lTarget->Name);
			return false;
		}
		else {
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Target is in range: ", lTarget->Name);
			return true;
		}
	}
		
	static void TargetByID(PSPAWNINFO pChar, char * aID) {
		char lCommand[MAX_STRING];
		sprintf_s(lCommand, "ID %s", aID);

		Target(pChar, lCommand);
	}

	static void TargetByID(PSPAWNINFO pChar, int aID) {
		char lCommand[MAX_STRING];
		sprintf_s(lCommand, "ID %d", aID);

		Target(pChar, lCommand);
	}

	static void MikuTargetEcho(char aColor, std::string aMessage, std::string aCast) {
		char lMessage[MAX_STRING];
		std::string lEchoName = "[Miku]";

		std::string lTargetName = "[No Target]";
		char lName[EQ_MAX_NAME];
		strcpy_s(lName, "");
		if (pTarget) {
			strcpy_s(lName, pTarget->Name);
		}
		sprintf_s(lMessage, "\a%c%s\ax %s \ap\"%s\" -> \ar%s", aColor, lEchoName.c_str(), aMessage.c_str(), aCast.c_str(), lName);
		WriteChatColor(lMessage);
	}

	static void MikuEcho(char aColor, std::string aMessage, std::string aTarget) {
		char lMessage[MAX_STRING];
		std::string lEchoName = "[Miku]";

		sprintf_s(lMessage, "\a%c%s\ax %s \ap\"%s\"", aColor, lEchoName.c_str(), aMessage.c_str(), aTarget.c_str());
		WriteChatColor(lMessage);
	}

	static void MikuEcho(char aColor, std::string aMessage, int aTarget) {
		MikuEcho(aColor, aMessage, std::to_string(aTarget));
	}

	static void MikuEcho(char aColor, std::string aMessage) {
		char lMessage[MAX_STRING];
		std::string lEchoName = "[Miku]";

		sprintf_s(lMessage, "\a%c%s\ax %s", aColor, lEchoName.c_str(), aMessage.c_str());
		WriteChatColor(lMessage);
	}

	static bool IsCastWindowShowing() {
		return
			(pCastingWnd && (CSidlScreenWnd*)pCastingWnd->dShow) ||
			(pCharSpawn && ((PSPAWNINFO)pCharSpawn)->CastingData.SpellID != -1);
	}
	
	static bool IsCasting() {
		bool lIsCasting = IsCastWindowShowing();

		if (!lIsCasting || (GetPcProfile()->Class != Bard)) return lIsCasting;

		PSPELL lSpell = GetSpellByID(((PSPAWNINFO)pCharSpawn)->CastingData.SpellID);

		if (!lSpell) return lIsCasting;

		return (lSpell->ClassLevel[GetPcProfile()->Class] == 255);
	}

	static bool IsCastingThis(DWORD aID) {
		return
			(pCastingWnd && (CSidlScreenWnd*)pCastingWnd->dShow) ||
			(pCharSpawn && ((PSPAWNINFO)pCharSpawn)->CastingData.SpellID == aID);
	}

	static void MikuSendCommand(std::string lCommandToSend)
	{
		char * lCommand = new char[lCommandToSend.length() + 1];
		strcpy_s(lCommand, lCommandToSend.length() + 1, lCommandToSend.c_str());

		EzCommand(lCommand);

		delete lCommand;
	}

	static long ActiveDiscipline() {
		PSPELL lSpell = GetSpellByName(pCombatAbilityWnd->GetChildItem("CAW_CombatEffectLabel")->GetWindowText().c_str());
		return (lSpell) ? lSpell->ID : 0;
	}

	static unsigned long GetClockTime() {
		return (unsigned long)clock();
	}

	static 	std::vector<std::string> GetParamList(char* aInput) {
		std::vector<std::string> lResult;

		char* lParsePtr;
		char* lDelimiter = " ";

		char* lValue = strtok_s(aInput, lDelimiter, &lParsePtr);

		if (lValue) {
			lResult.push_back(lValue);
		}

		while (lValue) {
			lValue = strtok_s(NULL, lDelimiter, &lParsePtr);

			if (lValue) {
				lResult.push_back(lValue);
			}
		}

		return lResult;
	}
};

#endif