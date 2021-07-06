#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"

using namespace std;

enum ArithemticCmd {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT}; 
enum Segment {CONSTANT, LOCAL, ARGUMENT, THIS, THAT, POINTER, TEMP};

class CodeWriter {
private:
	string default_file_name;
	ofstream asm_file;
	unordered_map<ArithemticCmd, string>* cmd2Asm;
	unordered_map<string, ArithemticCmd>* str2Amcmd;
	unordered_map<string, Segment>* str2Seg;
	unordered_map<Segment, string>* seg2Addr;
	string push_asm_str(string, int);
	string pop_asm_str(string, int);
public:
	CodeWriter(string);
	void setFileName(string);
	void writeArithmetic(string);
	void writePushPop(Command, string, int);	
	~CodeWriter();
};
