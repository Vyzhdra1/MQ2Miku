#pragma once
#include <string>
#include "StringParameter.h"

class BoolParameter : public StringParameter
{
public:
	BoolParameter(bool aValue) : StringParameter(aValue ? "Y" : "N") {}
};




