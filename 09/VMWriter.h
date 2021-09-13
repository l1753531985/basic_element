#include <fstream>
#include <unordered_map>
#include <iostream>
#include "Status.h"

using namespace std;

class VMWriter {
private:
	enum Segment {CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP};
	enum Command {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT};
	ofstream ofile;
	unordered_map<string, Status>* classScope;
	unordered_map<string, Status>* methodScope;
public:
	VMWriter();
	~VMWriter();
	void Constructor(string);
	void writePush(Segment, int);
	void writePop(Segment, int);
	void writeArithmetic(Command); 
	void writeLabel(string);
	void writeGoto(string);
	void writeIf(string);
	void writeCall(string, int);
	void writeFunction(string, int);
	void writeReturn();
	unordered_map<string, Status>* getClassScopePtr();
	unordered_map<string, Status>* getMethodScopePtr();
	void close();
	//for test
	void printScope(ostream&);
};
