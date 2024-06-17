#ifndef H_SkillAbility
#define H_SkillAbility
#include "Ability.h"

class SkillAbility : public Ability
{
private:
	unsigned int _SkillID = 0;
public:
	static const char* ConfigKey;

	SkillAbility() {}

	SkillAbility(std::string aKey, int aSkillID) {
		SetKey(aKey);
		SetName(aKey);
		_SkillID = aSkillID;
	}

	static bool HasSkill(unsigned int aSkillID) {
		if (aSkillID < 100 && (pSkillMgr->pSkill[aSkillID]->Activated && GetPcProfile()->Skill[aSkillID])) return true;
		if (aSkillID > 100 && aSkillID < 128 && GetPcProfile()->Skill[aSkillID] != 0xFF && strlen(szSkills[aSkillID]) > 3) return true;
		return false;
	}

	virtual bool IsLoaded() override {
		return true;
	}

	bool AbilityFound() {
		_SkillID = Utils::StrToInt(GetKey().c_str(), 0);
		return HasSkill(_SkillID);
	}

	bool AbilityReady() {
		return pSkillMgr->IsAvailable(_SkillID);
	}

	virtual bool CanCastSimultaneously() {
		return true;
	}

	bool Memorized() {
		return true;
	}

	virtual DWORD GetSpellID() {
		return _SkillID;
	}

	void Cast() {
		pCharData->UseSkill(_SkillID, pCharData->me);
	}

	void EchoCastMessage(char aSuccessType, char * aMessage) {
		Utils::MikuTargetEcho(aSuccessType, aMessage, GetName());
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded Skill: ", GetName());
	}

	virtual std::string GetType() {
		return SkillAbility::ConfigKey;
	}
};
const char* SkillAbility::ConfigKey = "skill";
#endif