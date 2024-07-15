#pragma once
#ifndef H_BooleanCondition
#define H_BooleanCondition
#include "Utils.h"
#include "Condition.h"
#include <set>

class BooleanCondition: public Condition
{
protected:
	bool _BooleanCondition;

	void Load(std::set<std::string> aSettings) {
		std::set<std::string>::iterator lIterator;
		for (lIterator = aSettings.begin(); lIterator != aSettings.end(); lIterator++)
		{
			if (!TRUE_STR.compare((*lIterator).c_str())) {
				_BooleanCondition = true;
			}
			else if (!FALSE_STR.compare((*lIterator).c_str())) {
				_BooleanCondition = false;
			}
			else {
				ParseNextValue((*lIterator));
			}
		}
	}
public:
	BooleanCondition(std::set<std::string> aSettings) { }
	BooleanCondition() { }

	virtual void ParseNextValue(std::string aValue) {
		Utils::MikuEcho(Utils::FAIL_COLOR, "Could not parse: ", aValue);
	}
};
#endif