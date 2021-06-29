#pragma once
#include <fstream>
#include <string> 
#include <unordered_map>

using namespace std;

enum Command {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, C_NOT};

class Parser{
private:
	ifstream ifs;
	string current_command;
	unordered_map<string, Command>* str2cmd;
	Command str2Cmd(string);
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
	string cmd2Str(Command);
};
