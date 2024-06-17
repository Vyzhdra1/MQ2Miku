#pragma once
#include "DbGear.h"
#include "DbManager.h"
#include "Utils.h"
#include <mq/Plugin.h>

class GearExporter {
private:

public:
	static void Export() {
		DbManager* lDbManager = DbManager::Get();

		lDbManager->GetGearContext()->DeleteAll();
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Successfully deleted gear data.");

		for (unsigned short lSlot = 0; lSlot < InvSlot_PowerSource; lSlot++) {
			DbGear* lGear = 0;

			ItemDefinition* lInfo = 0;

			ItemClient* lItem = GetPcProfile()->GetInventorySlot(lSlot);

			if (lItem) {
				lInfo = lItem->GetItemDefinition();
			}

			if (!lInfo) {
				lGear = new DbGear(
					lDbManager->GetCharacter()->GetCharacterID(),
					lSlot,
					-1,
					"<EMPTY>",
					-1,
					-1,
					-1,
					false,
					false
				);
			}
			else {
				lGear = new DbGear(
					lDbManager->GetCharacter()->GetCharacterID(),
					lSlot,
					lInfo->ItemNumber,
					lInfo->Name,
					lInfo->HP,
					lInfo->AC,
					lInfo->RequiredLevel,
					false,
					false
				);
			}
			lDbManager->GetGearContext()->AddGear(lGear);
		}

		lDbManager->GetGearContext()->SaveAll();
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Successfully exported gear data.");
	}
};