#ifndef __SYMBOLTABLE__
#define __SYMBOLTABLE__
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include "Status.h"

using namespace std;

enum KindType {STATIC, FIELD, ARG, VAR, NONE};

struct Status {
	string type;
	KindType kind; 
	int index;
	//for test
	static string KindType2Str(KindType kind)
	{
		switch (kind)
		{
			case KindType::STATIC:
				return "static";
			case KindType::FIELD:
				return "field";
			case KindType::ARG:
				return "arg";
			case KindType::VAR:
				return "var";
			default:
				return "none"; 	
		}
	}
};

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
	SymbolTable();
	~SymbolTable();
	void startSubroutine();
	void Define(string, string, KindType);
	int varCount(KindType);
	KindType kindOf(string);
	string TypeOf(string);
	int IndexOf(string);
	KindType str2Kind(string);
	//for test
	void printAllElem(ostream& os);
};
#endif
