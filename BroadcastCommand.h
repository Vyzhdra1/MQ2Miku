#ifndef H_BroadcastCommand
#define H_BroadcastCommand
#include "Utils.h"
#include "Parser.h"
#include "CommandConsts.h"

class BroadcastCommand
{
private:
	static const std::map<std::string, char* > COMMANDS;
	static std::map<std::string, char*> InitValidCommands()
	{
		std::map<std::string, char*> lResult;
		lResult["ass"] = "mikutar";
		lResult["bck"] = "mikuback";
		lResult["act"] = "mikuactivate";
		lResult["fol"] = "mikufol";
		lResult["atk"] = "mikuattk";
		lResult["burn"] = "mikuburn";
		return lResult;
	}
public:
	static bool Execute(PSPAWNINFO pChar, std::map<std::string, char*> aInput, MikuPlayer * aPlayer) {
		if (aInput.find(CommandConsts::COMMAND_KEY) == aInput.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Command Key in message");
			return false;
		}

		if (aInput.find(CommandConsts::BROADCAST_KEY) == aInput.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not find Broadcast Key in message");
			return false;
		}

		if (COMMANDS.find(aInput[CommandConsts::COMMAND_KEY]) == COMMANDS.end()) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Invalid command: ", aInput[CommandConsts::COMMAND_KEY]);
			return false;
		}

		std::string lExtraParams = "";

		for (std::map<std::string, char*>::iterator lIterator = aInput.begin(); lIterator != aInput.end(); lIterator++) {
			if (!_stricmp(lIterator->first.c_str(), CommandConsts::COMMAND_KEY)) {
				continue;
			}

			if (!_stricmp(lIterator->first.c_str(), CommandConsts::BROADCAST_KEY)) {
				continue;
			}

			lExtraParams.append(lIterator->first);
			lExtraParams.append("=");
			lExtraParams.append(lIterator->second);
			lExtraParams.append(";");
		}

		int lTargetID = pTarget ? pTarget->SpawnID : -1;
		int lSelfID = GetCharInfo()->pSpawn->SpawnID;

		char lCommandStr[MAX_STRING];
		sprintf_s(lCommandStr, "/%s /%s l=%d;t=%d;%s",
			aInput[CommandConsts::BROADCAST_KEY],
			BroadcastCommand::COMMANDS.at(aInput[CommandConsts::COMMAND_KEY]),
			lSelfID, lTargetID, lExtraParams.c_str());
		EzCommand(lCommandStr);
		return true;
	}

};
const std::map<std::string, char*> BroadcastCommand::COMMANDS = BroadcastCommand::InitValidCommands();
#endif