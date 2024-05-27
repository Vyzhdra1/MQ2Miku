// MQ2Miku.cpp : Defines the entry point for the DLL application.
//

// PLUGIN_API is only to be used for callbacks.  All existing callbacks at this time
// are shown below. Remove the ones your plugin does not use.  Always use Initialize
// and Shutdown for setup and cleanup.

#include <mq/Plugin.h>

PreSetup("MQ2Miku");
PLUGIN_VERSION(0.1);

#include "MikuPlayer.h"
#include "MikuClass.h"
#include "Utils.h"
#include "ConfigManager.h"

#include "BroadcastCommand.h"
#include "ActivateCommand.h"
#include "AssistCommand.h"
#include "AttackCommand.h"
#include "BurnCommand.h"
#include "FollowCommand.h"
#include "BackoffCommand.h"
#include "MikuMovementUtils.h"
#include "SettingManager.h"
#include "SpawnManager.h"
#include "HealingManager.h"
#include "MeleeUtils.h"
#include "DbManager.h"
#include "DbInitialiser.h"

//Setup stuff
MikuPlayer * _Player = 0;

//QBUFF
unsigned long gLastUpdate = 0;
unsigned long _NextUpdate = 0;

//_______________________________________________________________________________________
//
// Init 
//_______________________________________________________________________________________

void Init() {
    DebugSpew("Initialising");
    if (!GetPcProfile()) return;

	StickProc = (bool*)GetPluginProc("mq2moveutils", "bStickOn");

	GameManager::Init();
    long lClass = GetPcProfile()->Class;

    DebugSpew("Setting Class");
    switch (lClass) {
    case Shadowknight:
        _Player = new MikuShadowKnight();
        break;
    case Warrior:
        _Player = new MikuWarrior();
        break;
    case Rogue:
        _Player = new MikuRogue();
        break;
    case Monk:
        _Player = new MikuMonk();
        break;
    case Berserker:
        _Player = new MikuBerserker();
        break;
	case Beastlord:
		_Player = new MikuBeastlord();
		break;
    case Bard:
        _Player = new MikuBard();
        break;
    case Cleric:
        _Player = new MikuCleric();
        break;
    case Shaman:
        _Player = new MikuShaman();
        break;
    case Wizard:
        _Player = new MikuWizard();
        break;
    case Mage:
        _Player = new MikuMage();
        break;
    case Necromancer:
        _Player = new MikuNecromancer();
        break;
    case Enchanter:
        _Player = new MikuEnchanter();
        break;
	case Ranger:
		_Player = new MikuRanger();
		break;
	case Druid:
		_Player = new MikuDruid();
		break;
	case Paladin:
		_Player = new MikuPaladin();
		break;
    default:
		Utils::MikuEcho(Utils::FAIL_COLOR, "Could not load class. Class not found.");
		_Player = 0;
		return;
    }
    _Player->Init();

	MikuMovementUtils::UpdateLocation();
	DbInitialiser::Load(DbManager::Get(), _Player);


    ConfigManager * lConfig = new ConfigManager();
    lConfig->Load(_Player);

    DebugSpew("Class Set");
    delete lConfig;
}

void DeInit() {
    if (_Player) {
        delete _Player;
        _Player = 0;
    }

	SettingManager::Deinit();
	SpawnManager::Deinit();
	HealingManager::Deinit();
	AbilityManager::Deinit();
	GameManager::Deinit();
	DbManager::Deinit();
}

/**
 * @fn OnCleanUI
 *
 * This is called once just before the shutdown of the UI system and each time the
 * game requests that the UI be cleaned.  Most commonly this happens when a
 * /loadskin command is issued, but it also occurs when reaching the character
 * select screen and when first entering the game.
 *
 * One purpose of this function is to allow you to destroy any custom windows that
 * you have created and cleanup any UI items that need to be removed.
 */
PLUGIN_API void OnCleanUI()
{
	// DebugSpewAlways("MQ2Miku::OnCleanUI()");
}

/**
 * @fn OnReloadUI
 *
 * This is called once just after the UI system is loaded. Most commonly this
 * happens when a /loadskin command is issued, but it also occurs when first
 * entering the game.
 *
 * One purpose of this function is to allow you to recreate any custom windows
 * that you have setup.
 */
PLUGIN_API void OnReloadUI()
{
	// DebugSpewAlways("MQ2Miku::OnReloadUI()");
}

/**
 * @fn OnDrawHUD
 *
 * This is called each time the Heads Up Display (HUD) is drawn.  The HUD is
 * responsible for the net status and packet loss bar.
 *
 * Note that this is not called at all if the HUD is not shown (default F11 to
 * toggle).
 *
 * Because the net status is updated frequently, it is recommended to have a
 * timer or counter at the start of this call to limit the amount of times the
 * code in this section is executed.
 */
PLUGIN_API void OnDrawHUD()
{
/*
	static std::chrono::steady_clock::time_point DrawHUDTimer = std::chrono::steady_clock::now();
	// Run only after timer is up
	if (std::chrono::steady_clock::now() > DrawHUDTimer)
	{
		// Wait half a second before running again
		DrawHUDTimer = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
		DebugSpewAlways("MQ2Miku::OnDrawHUD()");
	}
*/
}

/**
 * @fn SetGameState
 *
 * This is called when the GameState changes.  It is also called once after the
 * plugin is initialized.
 *
 * For a list of known GameState values, see the constants that begin with
 * GAMESTATE_.  The most commonly used of these is GAMESTATE_INGAME.
 *
 * When zoning, this is called once after @ref OnBeginZone @ref OnRemoveSpawn
 * and @ref OnRemoveGroundItem are all done and then called once again after
 * @ref OnEndZone and @ref OnAddSpawn are done but prior to @ref OnAddGroundItem
 * and @ref OnZoned
 *
 * @param GameState int - The value of GameState at the time of the call
 */
PLUGIN_API void SetGameState(int GameState)
{
	// DebugSpewAlways("MQ2Miku::SetGameState(%d)", GameState);
	if (GameState == GAMESTATE_INGAME)
    {
        if (!_Player)
        {
            Init();

            DebugSpew("MQ2Melee SetGameState GAMESTATE_INGAME & NOT LOADED, calling Configure()");
        }
        else
        {
            DebugSpew("MQ2Melee SetGameState GAMESTATE_INGAME & LOADED=true, not calling anything");
        }
    }
    else if (GameState != GAMESTATE_LOGGINGIN)
    {
        if (_Player)
        {
            DebugSpew("MQ2Melee SetGameState not INGAME or LOGGINGIN & LOADED=true, setting LOADED false");
            DeInit();
        }
        else
        {
            DebugSpew("MQ2Melee SetGameState not INGAME and *is* LOGGINGIN, not changing loaded");
        }
    }
    DebugSpew("MQ2Melee SetGameState Loaded == %s", _Player ? "true" : "false");
}


/**
 * @fn OnPulse
 *
 * This is called each time MQ2 goes through its heartbeat (pulse) function.
 *
 * Because this happens very frequently, it is recommended to have a timer or
 * counter at the start of this call to limit the amount of times the code in
 * this section is executed.
 */
PLUGIN_API void OnPulse()
{

/*
	static std::chrono::steady_clock::time_point PulseTimer = std::chrono::steady_clock::now();
	// Run only after timer is up
	if (std::chrono::steady_clock::now() > PulseTimer)
	{
		// Wait 5 seconds before running again
		PulseTimer = std::chrono::steady_clock::now() + std::chrono::seconds(5);
		DebugSpewAlways("MQ2Miku::OnPulse()");
	}
*/
    if (gGameState != GAMESTATE_INGAME || !_Player) return;

	MeleeUtils::MonitorKeyPress();

	unsigned long lClockTime = Utils::GetClockTime();

	if (!_Player->IsBackOffInitiated()) {
		if (lClockTime < _NextUpdate) return;
	}

    _NextUpdate = lClockTime + SettingManager::Get()->GetUpdateDelay();

	SpawnManager::Get()->Assess();

    _Player->Pulse();

	MikuMovementUtils::UpdateLocation();
}

/**
 * @fn OnWriteChatColor
 *
 * This is called each time WriteChatColor is called (whether by MQ2Main or by any
 * plugin).  This can be considered the "when outputting text from MQ" callback.
 *
 * This ignores filters on display, so if they are needed either implement them in
 * this section or see @ref OnIncomingChat where filters are already handled.
 *
 * If CEverQuest::dsp_chat is not called, and events are required, they'll need to
 * be implemented here as well.  Otherwise, see @ref OnIncomingChat where that is
 * already handled.
 *
 * For a list of Color values, see the constants for USERCOLOR_.  The default is
 * USERCOLOR_DEFAULT.
 *
 * @param Line const char* - The line that was passed to WriteChatColor
 * @param Color int - The type of chat text this is to be sent as
 * @param Filter int - (default 0)
 */
PLUGIN_API void OnWriteChatColor(const char* Line, int Color, int Filter)
{
	// DebugSpewAlways("MQ2Miku::OnWriteChatColor(%s, %d, %d)", Line, Color, Filter);
}

/**
 * @fn OnIncomingChat
 *
 * This is called each time a line of chat is shown.  It occurs after MQ filters
 * and chat events have been handled.  If you need to know when MQ2 has sent chat,
 * consider using @ref OnWriteChatColor instead.
 *
 * For a list of Color values, see the constants for USERCOLOR_. The default is
 * USERCOLOR_DEFAULT.
 *
 * @param Line const char* - The line of text that was shown
 * @param Color int - The type of chat text this was sent as
 *
 * @return bool - Whether to filter this chat from display
 */
PLUGIN_API bool OnIncomingChat(const char* Line, DWORD Color)
{
	// DebugSpewAlways("MQ2Miku::OnIncomingChat(%s, %d)", Line, Color);
	if (gGameState != GAMESTATE_INGAME || !_Player) return 0;

	if (SettingManager::Get()->IsValueMatched(ROLE_STR, HEALER_STR)) return false;

	_Player->ParseChat(Line, Color);

	return false;
}

/**
 * @fn OnAddSpawn
 *
 * This is called each time a spawn is added to a zone (ie, something spawns). It is
 * also called for each existing spawn when a plugin first initializes.
 *
 * When zoning, this is called for all spawns in the zone after @ref OnEndZone is
 * called and before @ref OnZoned is called.
 *
 * @param pNewSpawn PSPAWNINFO - The spawn that was added
 */
PLUGIN_API void OnAddSpawn(PSPAWNINFO pNewSpawn)
{
	// DebugSpewAlways("MQ2Miku::OnAddSpawn(%s)", pNewSpawn->Name);
}

/**
 * @fn OnRemoveSpawn
 *
 * This is called each time a spawn is removed from a zone (ie, something despawns
 * or is killed).  It is NOT called when a plugin shuts down.
 *
 * When zoning, this is called for all spawns in the zone after @ref OnBeginZone is
 * called.
 *
 * @param pSpawn PSPAWNINFO - The spawn that was removed
 */
PLUGIN_API void OnRemoveSpawn(PSPAWNINFO pSpawn)
{
	// DebugSpewAlways("MQ2Miku::OnRemoveSpawn(%s)", pSpawn->Name);
}

/**
 * @fn OnAddGroundItem
 *
 * This is called each time a ground item is added to a zone (ie, something spawns).
 * It is also called for each existing ground item when a plugin first initializes.
 *
 * When zoning, this is called for all ground items in the zone after @ref OnEndZone
 * is called and before @ref OnZoned is called.
 *
 * @param pNewGroundItem PGROUNDITEM - The ground item that was added
 */
PLUGIN_API void OnAddGroundItem(PGROUNDITEM pNewGroundItem)
{
	// DebugSpewAlways("MQ2Miku::OnAddGroundItem(%d)", pNewGroundItem->DropID);
}

/**
 * @fn OnRemoveGroundItem
 *
 * This is called each time a ground item is removed from a zone (ie, something
 * despawns or is picked up).  It is NOT called when a plugin shuts down.
 *
 * When zoning, this is called for all ground items in the zone after
 * @ref OnBeginZone is called.
 *
 * @param pGroundItem PGROUNDITEM - The ground item that was removed
 */
PLUGIN_API void OnRemoveGroundItem(PGROUNDITEM pGroundItem)
{
	// DebugSpewAlways("MQ2Miku::OnRemoveGroundItem(%d)", pGroundItem->DropID);
}

/**
 * @fn OnZoned
 *
 * This is called after entering a new zone and the zone is considered "loaded."
 *
 * It occurs after @ref OnEndZone @ref OnAddSpawn and @ref OnAddGroundItem have
 * been called.
 */
PLUGIN_API void OnZoned()
{
	// DebugSpewAlways("MQ2Miku::OnZoned()");
}

/**
 * @fn OnUpdateImGui
 *
 * This is called each time that the ImGui Overlay is rendered. Use this to render
 * and update plugin specific widgets.
 *
 * Because this happens extremely frequently, it is recommended to move any actual
 * work to a separate call and use this only for updating the display.
 */
PLUGIN_API void OnUpdateImGui()
{
/*
	if (GetGameState() == GAMESTATE_INGAME)
	{
		if (ShowMQ2MikuWindow)
		{
			if (ImGui::Begin("MQ2Miku", &ShowMQ2MikuWindow, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::Text("MQ2Miku is loaded!");
					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}
	}
*/
}

/**
 * @fn OnMacroStart
 *
 * This is called each time a macro starts (ex: /mac somemacro.mac), prior to
 * launching the macro.
 *
 * @param Name const char* - The name of the macro that was launched
 */
PLUGIN_API void OnMacroStart(const char* Name)
{
	// DebugSpewAlways("MQ2Miku::OnMacroStart(%s)", Name);
}

/**
 * @fn OnMacroStop
 *
 * This is called each time a macro stops (ex: /endmac), after the macro has ended.
 *
 * @param Name const char* - The name of the macro that was stopped.
 */
PLUGIN_API void OnMacroStop(const char* Name)
{
	// DebugSpewAlways("MQ2Miku::OnMacroStop(%s)", Name);
}

/**
 * @fn OnLoadPlugin
 *
 * This is called each time a plugin is loaded (ex: /plugin someplugin), after the
 * plugin has been loaded and any associated -AutoExec.cfg file has been launched.
 * This means it will be executed after the plugin's @ref InitializePlugin callback.
 *
 * This is also called when THIS plugin is loaded, but initialization tasks should
 * still be done in @ref InitializePlugin.
 *
 * @param Name const char* - The name of the plugin that was loaded
 */
PLUGIN_API void OnLoadPlugin(const char* Name)
{
	// DebugSpewAlways("MQ2Miku::OnLoadPlugin(%s)", Name);
}

/**
 * @fn OnUnloadPlugin
 *
 * This is called each time a plugin is unloaded (ex: /plugin someplugin unload),
 * just prior to the plugin unloading.  This means it will be executed prior to that
 * plugin's @ref ShutdownPlugin callback.
 *
 * This is also called when THIS plugin is unloaded, but shutdown tasks should still
 * be done in @ref ShutdownPlugin.
 *
 * @param Name const char* - The name of the plugin that is to be unloaded
 */
PLUGIN_API void OnUnloadPlugin(const char* Name)
{
	// DebugSpewAlways("MQ2Miku::OnUnloadPlugin(%s)", Name);
}

//_______________________________________________________________________________________
//
// Commands
//_______________________________________________________________________________________

typedef bool(*CommandExecute)(PSPAWNINFO pChar, std::map<std::string, char*>, MikuPlayer*);
void HandleCommand(PSPAWNINFO pChar, char * szLine, CommandExecute aExecute) {
	std::map<std::string, char*> lCommand = Parser::GetKeyValuePair(szLine);

	int lRange = SettingManager::Get()->GetAssistRange();

	if (lCommand.find(CommandConsts::RANGE_KEY) != lCommand.end()) {
		lRange = Utils::StrToInt(lCommand[CommandConsts::RANGE_KEY], lRange);
	}

    if (lCommand.find(CommandConsts::LEADER_KEY) != lCommand.end()) {
        if (!Utils::IsTargetInRange(pChar, lCommand[CommandConsts::LEADER_KEY], lRange)) {
            Utils::MikuEcho(Utils::FAIL_COLOR, "Broadcaster out of range.");
            return;
        }
    }

    if (!(aExecute == BroadcastCommand::Execute) && !PlayerUtils::CanExecuteAbility()) {
        Utils::MikuEcho(Utils::FAIL_COLOR, "Cannot execute command as you are either Dead or Feigned.");
        return;
    }

    aExecute(pChar, lCommand, _Player);
}

void MikuTarget(PSPAWNINFO pChar, PCHAR szLine) {
    HandleCommand(pChar, szLine, AssistCommand::Execute);
}

void MikuAttack(PSPAWNINFO pChar, PCHAR szLine) {
    HandleCommand(pChar, szLine, AttackCommand::Execute);
}

void MikuBackOff(PSPAWNINFO pChar, PCHAR szLine) {
    HandleCommand(pChar, szLine, BackoffCommand::Execute);
}

void MikuBurn(PSPAWNINFO pChar, PCHAR szLine) {
	HandleCommand(pChar, szLine, BurnCommand::Execute);
}

void MikuFollow(PSPAWNINFO pChar, PCHAR szLine) {
    HandleCommand(pChar, szLine, FollowCommand::Execute);
}

void MikuActivate(PSPAWNINFO pChar, PCHAR szLine)
{
    HandleCommand(pChar, szLine, ActivateCommand::Execute);
}

void MikuReload(PSPAWNINFO pChar, PCHAR szLine)
{
    DeInit();
    Init();
}

void MikuBroadcast(PSPAWNINFO pChar, PCHAR szLine)
{
    HandleCommand(pChar, szLine, BroadcastCommand::Execute);
}

void MikuBroadcastFollow(PSPAWNINFO pChar, PCHAR szLine)
{
	char lCommandStr[] = "c=fol;b=dge;";
	char* lCommandStrPtr = &lCommandStr[0];
	HandleCommand(pChar, lCommandStrPtr, BroadcastCommand::Execute);
}

void MikuTest(PSPAWNINFO pChar, PCHAR szLine)
{
	//GlobalWorkspace::Get()->GetSettingsContext()->Load();
}

void MikuSet(PSPAWNINFO pChar, PCHAR szLine) {
	SettingManager::Get()->ParseCommand(szLine);
}

void MikuReport(PSPAWNINFO pChar, PCHAR szLine)
{
	_Player->Report();
}

PLUGIN_API VOID OnEndZone(VOID)
{
    if (!_Player) return;

	AbilityManager::Get()->Refresh();

	_NextUpdate = Utils::GetClockTime() + (SettingManager::Get()->GetUpdateDelay() * 5);
}

PLUGIN_API VOID OnBeginZone(VOID)
{
    

//  DeInit();
}

void MikuReportX(std::string aReportType) {
	std::string LOWSPELLS_STR = "lowspells";

	if (!LOWSPELLS_STR.compare(aReportType)) {
		AbilityManager::Get()->ReportLowSpells();
		return;
	}
	else {
		Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown Report Type Command: ", aReportType);
	}
}

void MikuAction(PSPAWNINFO pChar, PCHAR szLine) {
	std::string REPORT_STR = "report";
	std::string REGISTER_STR = "register";

	std::vector<std::string> lParams = Utils::GetParamList(szLine);

	std::vector<std::string>::iterator lIterator = lParams.begin();

	if (lIterator == lParams.end()) {
		Utils::MikuEcho(Utils::FAIL_COLOR, "No parameter found");
		return;
	}

	if (lParams.size() < 2) {
		Utils::MikuEcho(Utils::FAIL_COLOR, "Usage: /miku [Action] [ActionArgument]");
		return;
	}

	std::string lAction = *lIterator;
	lIterator++;
	std::string lActionArgument = *lIterator;


	if (!REPORT_STR.compare(lAction)) {
		MikuReportX(lActionArgument);
		return;
	}
	else if (!REGISTER_STR.compare(lAction)) {
		HealingManager::Get()->RegisterTarget(lActionArgument);
		return;
	}
	else {
		Utils::MikuEcho(Utils::FAIL_COLOR, "Unknown Command: ", lAction);
	}
}

// Called once, when the plugin is to initialize
PLUGIN_API VOID InitializePlugin(VOID)
{
    DebugSpewAlways("Initializing MQ2Miku");
	AddCommand("/mikutest", MikuTest);
	AddCommand("/miku", MikuAction);
    AddCommand("/mikubc", MikuBroadcast);
    AddCommand("/mikuattk", MikuAttack);
	AddCommand("/mikuburn", MikuBurn);
    AddCommand("/mikutar", MikuTarget);
    AddCommand("/mikufol", MikuFollow);
    AddCommand("/mikuback", MikuBackOff);
    AddCommand("/mikureload", MikuReload);
    AddCommand("/mikuactivate", MikuActivate);
	AddCommand("/mikureport", MikuReport);
	AddCommand("/mikuset", MikuSet);
}

// Called once, when the plugin is to shutdown
PLUGIN_API VOID ShutdownPlugin(VOID)
{
    DebugSpewAlways("Shutting down MQ2Miku");
	RemoveCommand("/miku");
	RemoveCommand("/mikutest");
    RemoveCommand("/mikubc");;
	RemoveCommand("/mikuburn");
    RemoveCommand("/mikuattk");
    RemoveCommand("/mikutar");
    RemoveCommand("/mikufol");
    RemoveCommand("/mikuback");
    RemoveCommand("/mikureload");
    RemoveCommand("/mikuactivate");
	RemoveCommand("/mikureport");
	RemoveCommand("/mikuset");

    DeInit();
}