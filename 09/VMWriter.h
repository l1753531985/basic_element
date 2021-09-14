#include <fstream>
#include <unordered_map>
#include <iostream>
#include "SymbolTable.h"

using namespace std;

class VMWriter {
private:
	enum Segment {CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP};
	enum Command {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT};
	ofstream ofile;
	string fileName;
public:
	VMWriter(string);
	~VMWriter();
	void Constructor();
	void writePush(Segment, int);
	void writePop(Segment, int);
	void writeArithmetic(Command); 
	void writeLabel(string);
	void writeGoto(string);
	void writeIf(string);
	void writeCall(string, int);
	void writeFunction(string, int);
	void writeReturn();
	void close();
};
