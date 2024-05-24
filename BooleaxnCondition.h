#ifndef H_BoozleanCondition
#define H_BoozleanCondition
#include "Utils.h"
#include <set>


/*
class BooleanCondition 
{
protected:
	bool _BooleanCondition;
public:
	BooleanCondition(std::set<std::string> aSettings) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			if (!Utils::TRUE_STR.compare((*lIterator).c_str())) {
				_BooleanCondition = true;
			}
			else if (!Utils::FALSE_STR.compare((*lIterator).c_str())) {
				_BooleanCondition = false;
			}
			else {
				ParseNextValue((*lIterator));
			}
		}
	}

	virtual void ParseNextValue(std::string aValue) {
		Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
	}
};*/
#endif
