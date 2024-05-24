#ifndef H_MikuRogue
#define H_MikuRogue
#include "ClassRole.h"
#include "Utils.h"

class MikuShaman : public HealerRole
{
private:
	void LoadAA();
public:
	virtual void Init() override;
	void PerformHealLogic();
};

void MikuShaman::Init() {
	_ClassName = "ROG";
	HealerRole::Init();
}

void MikuShaman::LoadAA() {

}

void MikuShaman::PerformHealLogic() {
	//If Tank.Health < 90 then Cast Reckless
	// 
	//If Average Group HP Excluding Tank < 90 Then Cast Group Heals
	// When to use cooldowns?
	// 
	//if not slowed then Slow
	//Else Dot/DD


}

#endif
#pragma once
