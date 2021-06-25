#pragma once
#include <string>
#include <fstream>
#include <iostream>

enum Command {A_COMMAND, C_COMMAND, L_COMMAND, NOT_A_COMMAND};

using namespace std;

class Parser {
private:
	fstream ifile;
	string current_command;
	Command whichType(string);
public:
	Parser();
	Parser(string);
	~Parser();
	bool initFileSuccess();
	bool hasMoreCommands();
	void advance();
	bool commandIsEmpty();
	Command commandType();
	string symbol();
	string dest();
	string comp();
	string jump();
	//test function
	string get_current_command() { return current_command; }
	string command2string(Command); 
};
