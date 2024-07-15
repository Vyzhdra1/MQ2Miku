#ifndef H_Parser
#define H_Parser

class Parser
{

public:	
	static std::set<std::string> GetSet(char * aInput) {
		char * lParsePtr;
		char * lValueDelimiter = ",";
		char * lKey = strtok_s(aInput, lValueDelimiter, &lParsePtr);

		std::set<std::string> lResult;

		while (lKey) {
			lResult.insert(lKey);

			lKey = strtok_s(NULL, lValueDelimiter, &lParsePtr);
		}

		return lResult;
	}

	static std::map<std::string, char*> GetKeyValuePair(char * aInput) {
		std::map<std::string, char*> lResult;

		if (!strlen(aInput)) {
			return lResult;
		}

		char * lParsePtr;
		char * lNameDelimiter = "=";
		char * lValueDelimiter = ";";
		char * lKey = strtok_s(aInput, lNameDelimiter, &lParsePtr);


		while (lKey) {
			char * lValue = strtok_s(NULL, lValueDelimiter, &lParsePtr);

			if (lKey != 0 && lValue != 0) {
				lResult.insert(std::make_pair(lKey, lValue));
			}

			lKey = strtok_s(NULL, lNameDelimiter, &lParsePtr);
		}

		return lResult;
	}
};
#endif