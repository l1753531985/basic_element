#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include "Status.h"

using namespace std;

class SymbolTable {
private:
	unordered_map<string, Status>* classScope;
	unordered_map<string, Status>* methodScope;
	unordered_map<string, KindType> str2KindType;
	int staticSegCount;
	int fieldSegCount;
	int argSegCount;
	int varSegCount;
	unordered_map<string, Status>::iterator getIter(string);
public:
	SymbolTable(unordered_map<string, Status>*, unordered_map<string, Status>*);
	~SymbolTable();
	void startSubroutine();
	void Define(string, string, KindType);
	int varCount(KindType);
	KindType kindOf(string);
	string TypeOf(string);
	int IndexOf(string);
	void giveDataForStore(unordered_map<string, pair<string, string>>*, queue<string>&);
};
