#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "Parser.h"

using namespace std;

enum ArithemticCmd {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT}; 
enum Segment {CONSTANT, LOCAL, ARGUMENT, THIS, THAT, POINTER, TEMP, STATIC};
enum ControlCmd {LABEL, IF, GOTO};

class CodeWriter {
private:
	string default_file_name;
	ofstream asm_file;
	int BRANCH_FLAG;
	int WRITE_FLAG;
	int RET_FLAG;
	unordered_map<ArithemticCmd, string>* cmd2Asm;
	unordered_map<string, ArithemticCmd>* str2Amcmd;
	unordered_map<string, Segment>* str2Seg;
	unordered_map<Segment, string>* seg2Addr;
	string push_asm_str(string, int);
	string pop_asm_str(string, int);
	string getCmpStr(string);
	string getOpStr(string);
	string label2std(string);
public:
	CodeWriter(string);
	void setFileName(string);
	void writeArithmetic(string);
	void writePushPop(Command, string, int);	
	void writeControl(Command, string);
	void writeFunction(string, int);
	void writeRuturn();
	void writeCall(string, int);
	~CodeWriter();
};
