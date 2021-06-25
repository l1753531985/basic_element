#pragma once
#include <fstream>
#include <string> 

using namespace std;

enum Command {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCITON, C_RETURN, C_CALL};

class Parser{
private:
	ifstream ifs;
	string current_command;
public:
	Parser();
	Parser(string);
	bool initFileSuccess();
	~Parser();
	bool hasMoreCommands();
	void advance();
	Command commandType();
	string arg1();
	int arg2();
	// test
	string getCommand();
};
