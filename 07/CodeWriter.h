#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"

using namespace std;

enum ArithemticCmd {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT}; 
enum Segment {CONSTANT, LOCAL, ARGUMENT, THIS, THAT, POINTER};

class CodeWriter {
private:
	string default_file_name;
	ofstream asm_file;
	unordered_map<ArithemticCmd, string>* cmd2Asm;
	unordered_map<string, ArithemticCmd>* str2Amcmd;
	unordered_map<string, Segment>* str2Seg;
public:
	CodeWriter(string);
	void setFileName(string);
	void writeArithmetic(string);
	void writePushPop(Command, string, int);	
	~CodeWriter();
};
