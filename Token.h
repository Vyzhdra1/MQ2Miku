#pragma once
#include <string>
#ifndef H_Token
#define H_Token

class Token
{
private:
	std::string _Key;
	std::set<std::string> _Settings;
public:
	std::string GetKey();

	std::set<std::string> GetSettings() {
		return _Settings;
	}

	Token(std::string aKey, std::string aValue) {
		_Key = aKey;
		InitSettings(aValue);
	}
	
	void InitSettings(std::string aInput) {
		std::string lInput = aInput;
		std::string lDelimiter = ",";

		size_t pos = 0;
		std::string lToken;
		while ((pos = lInput.find(lDelimiter)) != std::string::npos) {
			lToken = lInput.substr(0, pos);
			_Settings.insert(lToken);
			lInput.erase(0, pos + lDelimiter.length());
		}

		if (lInput.compare("")) {
			_Settings.insert(lInput);
		}
	}
};
std::string Token::GetKey() {
	return _Key;
}
#endif