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
	unordered_map<string, Status>* classScope;
	unordered_map<string, Status>* methodScope;
	int staticSegCount;
	int fieldSegCount;
	int argSegCount;
	int varSegCount;
public:
	SymbolTable();
	~SymbolTable();
	void startSubroutine();
	void Define(string, string, KindType);
	int varCount(KindType);
	KindType kindOf(string);
	string TypeOf(string);
	int IndexOf(string);
};
