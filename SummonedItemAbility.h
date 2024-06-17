#ifndef H_SummonedItemAbility
#define H_SummonedItemAbility
#include "ItemAbility.h"

class SummonedItemAbility : public ItemAbility
{
public:
	static const char * ConfigKey;

	SummonedItemAbility() : ItemAbility() {
		Silence();
	}

	virtual bool IsLoaded() override {
		return true;
	}

	bool AbilityFound()  {
		return true;
	}

	bool AbilityReady() {
		return ItemAbility::AbilityFound() && ItemAbility::AbilityReady();
	}

	virtual void EchoLoadSuccessMessage() {
		Utils::MikuEcho(Utils::SUCCESS_COLOR, "Loaded SummonedItem: ", GetKey());
	}

	virtual std::string GetType() {
		return SummonedItemAbility::ConfigKey;
	}
};
const char * SummonedItemAbility::ConfigKey = "summoned";
#endif