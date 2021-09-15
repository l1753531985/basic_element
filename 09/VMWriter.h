#ifndef __VMWRITER__
#define __VMWRITER__

#include <fstream>
#include <unordered_map>
#include <iostream>
#include "SymbolTable.h"

using namespace std;

enum Segment {S_CONST, S_ARG, S_LOCAL, S_STATIC, S_THIS, S_THAT, S_POINTER, S_TEMP};

class VMWriter {
private:
	enum Command {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT};
	ofstream ofile;
	string fileName;
	unordered_map<Segment, string> seg2Str;
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

#endif
