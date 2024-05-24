#ifndef H_SkillAbility
#define H_SkillAbility
#include "Ability.h"

class SkillAbility : public Ability
{
private:
	PALTABILITY _Ability;
	unsigned int _SkillID = 0;
public:
	static const char* ConfigKey;

	bool AbilityFound() {
		_SkillID = Utils::StrToInt(GetKey(), 0);
		if (_SkillID < 100 && (pSkillMgr->pSkill[_SkillID]->Activated && GetPcProfile()->Skill[_SkillID])) return true;
		if (_SkillID > 100 && _SkillID < 128 && GetPcProfile()->Skill[_SkillID] != 0xFF && strlen(szSkills[_SkillID]) > 3) return true;
		return false;
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