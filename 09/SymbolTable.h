#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

using namespace std;

enum KindType {STATIC, FIELD, ARG, VAR, NONE};

struct Status {
	string type;
	KindType kind; 
	int index;
};

class SymbolTable {
private:
	unordered_map<string, pair<string, string>>* data;
	unordered_map<string, Status>* classScope;
	unordered_map<string, Status>* methodScope;
	unordered_map<string, KindType> str2KindType;
	int staticSegCount;
	int fieldSegCount;
	int argSegCount;
	int varSegCount;
	unordered_map<string, Status>::iterator getIter(string);
	queue<string> identifiersInOrder;
public:
	SymbolTable(const queue<string>&);
	~SymbolTable();
	void startSubroutine();
	void Define(string, string, KindType);
	int varCount(KindType);
	KindType kindOf(string);
	string TypeOf(string);
	int IndexOf(string);
	void getData(unordered_map<string, pair<string, string>>*);
	//for test
	void printData();
};
