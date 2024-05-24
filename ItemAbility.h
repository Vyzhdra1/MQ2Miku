#ifndef H_ItemAbility
#define H_ItemAbility
#include "Ability.h"

class ItemAbility : public Ability
{
private:
	DWORD _Slot = -1;
	DWORD _Container = -1;
	PCONTENTS _Item;
	bool _Silent = false;
public:
	static const char* ConfigKey;

	void Silence() {
		_Silent = true;
	}

	bool GetItemNumberByPartialSpellName(PITEMINFO aInfo, unsigned long aSlot, unsigned long aContainerSlot = -1) {
		DWORD lSpellID = aInfo->Clicky.SpellID;

		PSPELL pSpell = GetSpellByID(lSpellID);
		if (!(pSpell)) return false;

		std::string lSpellName(pSpell->Name);
		size_t lMatch = lSpellName.find(GetKey());
		if (lMatch == std::string::npos) return false;

		_Slot = aSlot;
		_Container = aContainerSlot;
		_Spell = pSpell;
	//	Utils::MikuEcho('g', "Loaded Item: ", aInfo->Name);
		return true;
	}

	bool GetItemInfo(PCONTENTS aContents, PCONTENTS &Item, PITEMINFO &aInfo) {
		Item = aContents;
		if (!aContents)  {
			aInfo = 0;
			return false;
		}

		aInfo = GetItemFromContents(Item);
		return aInfo != 0;
	}

	virtual void Refresh() {
		AbilityFound();
	}

	bool AbilityFound()  {
		_Slot = -1;
		_Container = -1;

		PCONTENTS lItemLvl1 = 0;
		PITEMINFO lInfoLvl1 = 0;

		PCONTENTS lItemLvl2 = 0;
		PITEMINFO lInfoLvl2 = 0;

		PcProfile* pChar2 = GetPcProfile();
		for (unsigned short usSlot = 0; usSlot < NUM_INV_SLOTS; usSlot++) {

			//pChar2->pInventoryArray->InventoryArray[usSlot]
			if (!GetItemInfo(pChar2->GetInventorySlot(usSlot), lItemLvl1, lInfoLvl1)) continue;

			if (lInfoLvl1->Type == ITEMTYPE_PACK && lItemLvl1->Contents.ContainedItems.pItems) {
				for (unsigned long nItem = 0; nItem < lInfoLvl1->Slots; nItem++) {
					if (!GetItemInfo(lItemLvl1->GetContent(nItem), lItemLvl2, lInfoLvl2)) continue;

					if (GetItemNumberByPartialSpellName(lInfoLvl2, nItem, usSlot)) {
						_Item = lItemLvl2;
						return true;
					}
				}
			}
			else {
				if (GetItemNumberByPartialSpellName(lInfoLvl1, usSlot)) {
					_Item = lItemLvl1;
					return true;
				}
			}
		}

		if (!_Silent) {
			Utils::MikuEcho('r', "Could not find item: ", GetKey());
		}
		return false;
	}

	bool Memorized() {
		return true;
	}

	long ItemTimer(PCONTENTS pItem) {
		ItemDefinition* lItem = GetItemFromContents(pItem);

		if (!lItem) return 999999;

		if (lItem->Clicky.TimerID != 0xFFFFFFFF) return GetItemTimer(pItem);
		if (lItem->Clicky.SpellID != 0xFFFFFFFF) return 0;
		return 999999;
	}
	
	bool AbilityReady() {
		try {
			bool lItemReady = ItemTimer(_Item) == 0;
			return lItemReady;
		} 
		catch (int x) {
			Utils::MikuEcho(Utils::FAIL_COLOR, "Could not determine Item Timer: ", GetKey());
			return true;
		}
	}

	void Cast() {
		char lCommand[MAX_STRING];
		if (_Container == -1)
			sprintf_s(lCommand, "/useitem %d", _Slot);
		else
			sprintf_s(lCommand, "/useitem %d %d", _Container, _Slot);
		EzCommand(lCommand);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Spell->Name);
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Item: ", _Spell->Name);
	}

	virtual std::string GetType() {
		return ItemAbility::ConfigKey;
	}
};
const char* ItemAbility::ConfigKey = "item";
#endif