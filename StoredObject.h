#pragma once

class StoredObject {
private:  
	enum StoredStatus { INSERTED, DELETED, UPDATED };
public: 
	 bool virtual Save() = 0;
	 bool virtual Delete() = 0;

};