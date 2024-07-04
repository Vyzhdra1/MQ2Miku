#pragma once
#include "Ability.h"
#include "DbItemAbility.h"

class ItemAbility : public Ability
{
private:
	ItemClient* _ItemClient = 0;
	ItemDefinition* _ItemDefinition = 0;
	bool _Silent = false;

	int _PrimarySlot = -1;
	int _SecondarySlot = -1;

	int _ItemID = -1;
	int _AlternateItemID = -1;
	int _SPA = -1;
	int _ItemSlot = -1;
	int _RecastType = -1;
	bool _IsSummoned = false;
public:
	static const char* ConfigKey;

	ItemAbility() {}

	ItemAbility(DbItemAbility* aDbItemAbility) {
		SetKey(aDbItemAbility->GetItemKey());
		SetName(aDbItemAbility->GetItemKey());

		_ItemID = aDbItemAbility->GetItemID();
		_AlternateItemID = aDbItemAbility->GetAlternateItemID();
		_SPA = aDbItemAbility->GetSPA();
		_ItemSlot = aDbItemAbility->GetItemSlot();
		_RecastType = aDbItemAbility->GetRecastType();
		_IsSummoned = aDbItemAbility->GetIsSummoned();

		_IsBuff = aDbItemAbility->GetIsBuff();

		ReloadItem();
	}

	void ReloadItem() {
		_ItemClient = 0;
		_ItemDefinition = 0;
		_Spell = 0;

		FindItem(_ItemID, _AlternateItemID, _ItemSlot, _SPA, _RecastType, &_PrimarySlot, &_SecondarySlot);

		InitItemDefinition();

		if (!_ItemDefinition) return;

		_Spell = GetSpellByID(_ItemDefinition->GetSpellData(ItemSpellType_Clicky)->SpellID);
		_ItemID = _ItemDefinition->ItemNumber;
	}

	void InitItemDefinition() {
		_ItemClient = 0;
		_ItemDefinition = 0;

		if (_PrimarySlot == -1) return;

		_ItemClient = GetPcProfile()->GetInventorySlot(_PrimarySlot);


		if (!_ItemClient) return;

		_ItemDefinition = _ItemClient->GetItemDefinition();

		if (_SecondarySlot != -1) {
			_ItemClient = _ItemClient->GetChildItemContainer()->GetItem(_SecondarySlot);

			if (_ItemClient) {
				_ItemDefinition = _ItemClient->GetItemDefinition();
			}
		}
	}

	bool IsItemMatch(
		int aItemID,
		int aAlternateItemID,
		int aItemSlot,
		int aSPA,
		int aRecastType,
		int aPrimarySlot,
		ItemDefinition* aInfo) {

		if (!aInfo) return false;

		if (aInfo->GetSpellData(ItemSpellType_Clicky)->SpellID <= 0) return false;

		if ((aItemSlot != -1) && (aPrimarySlot != aItemSlot))  return false;

		if ((aItemID != -1) || (aAlternateItemID != -1)) {
			return (aInfo->ItemNumber == aAlternateItemID) || (aInfo->ItemNumber == aItemID);
		}

		if ((aRecastType != -1) && (aRecastType != aInfo->GetSpellData(ItemSpellType_Clicky)->RecastType))  return false;

		if (aSPA != -1) {
			PSPELL lSpell = GetSpellByID(aInfo->GetSpellData(ItemSpellType_Clicky)->SpellID);
			if (!lSpell || !IsSPAEffect(lSpell, aSPA)) {
				return false;
			}
		}

		return true;
	}

	bool GetItemMatch(
		int aItemID,
		int aAlternateItemID,
		int aItemSlot,
		int aSPA,
		int aRecastType,
		int aPrimarySlot, 
		int* aSecondarySlot) {
		ItemClient* lItem = GetPcProfile()->GetInventorySlot(aPrimarySlot);

		if (!lItem) return false;

		ItemDefinition* lInfo = 0;
		if (lItem) {
			lInfo = lItem->GetItemDefinition();
		}

		if (!lItem->IsContainer()) {
			if (IsItemMatch(aItemID, aAlternateItemID, aItemSlot, aSPA, aRecastType, aPrimarySlot, lInfo)) {
				*aSecondarySlot = -1;
				return true;
			}
			return false;
		}	

		for (unsigned short lContainerSlots = 0; lContainerSlots < lInfo->Slots; lContainerSlots++) {
			ItemClient* lContainerSubItem = lItem->GetChildItemContainer()->GetItem(lContainerSlots);

			if (!lContainerSubItem) continue;

			ItemDefinition* lContainerSubInfo = 0;
			if (lContainerSubItem) {
				lContainerSubInfo = lContainerSubItem->GetItemDefinition();
			}

			if (IsItemMatch(aItemID, aAlternateItemID, aItemSlot, aSPA, aRecastType, aPrimarySlot, lContainerSubInfo)) {
				*aSecondarySlot = lContainerSlots;
				return true;
			}
		}
		return false;
	}	

	void FindItem(
		int aItemID,
		int aAlternateItemID,
		int aItemSlot,
		int aSPA,
		int aRecastType,
		int* aPrimarySlot,
		int* aSecondarySlot
	) {
		*aPrimarySlot = -1;
		*aSecondarySlot = -1;
		for (unsigned short lSlot = 0; lSlot < InvSlot_NumInvSlots; lSlot++) {
			if (GetItemMatch(aItemID, aAlternateItemID, aItemSlot, aSPA, aRecastType, lSlot, aSecondarySlot)) {
				*aPrimarySlot = lSlot;
				return;
			}
		}
	}

	void Silence() {
		_Silent = true;
	}

	virtual bool IsLoaded() override {
		return _IsSummoned || _ItemClient;
	}

	virtual void Refresh() {
		ReloadItem();
	}

	bool ItemFound() {
		return _ItemDefinition;
	}

	bool AbilityFound()  {
		if (_ItemID) {
			InitItemDefinition();

			if (_ItemDefinition && (_ItemID == _ItemDefinition->ItemNumber)) return true;
		}

		ReloadItem();

		return _ItemDefinition;
	}

	bool Memorized() {
		return true;
	}

	long ItemTimer(PCONTENTS pItem) {
		if (!_ItemDefinition) return 999999;

		return GetItemTimer(_ItemClient);
	}
	
	bool AbilityReady() {
		AbilityFound();

		if (!_ItemDefinition) return false;

		return GetItemTimer(_ItemClient) == 0;
	}

	void Cast() {
		char lCommand[MAX_STRING];
		if (_SecondarySlot == -1)
			sprintf_s(lCommand, "/useitem %d", _PrimarySlot);
		else
			sprintf_s(lCommand, "/useitem %d %d", _PrimarySlot, _SecondarySlot);
		EzCommand(lCommand);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, _Spell->Name);
	}

	virtual void EchoLoadSuccessMessage() {
		if (_ItemDefinition) {
			std::string lMessage = 
				"Key: " + GetKey() + " | Spell: " + _Spell->Name + " | Item: " + 
				_ItemDefinition->Name + " | Primary: " + std::to_string(_PrimarySlot) + " | Secondary: " + std::to_string(_SecondarySlot);
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Item: ", lMessage);
		}
		else {
			std::string lMessage = "Key: " + GetKey();
			Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded TempItem: ", lMessage);
		}
	}
};
const char* ItemAbility::ConfigKey = "item";
