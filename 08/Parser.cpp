#include "Parser.h"
#include <iostream>

using namespace std;

Parser::Parser():current_command{""},str2cmd{nullptr} {}

Parser::Parser(string file)
	:current_command{""}, str2cmd{new unordered_map<string, Command>} 
{
	ifs.open(file);
	str2cmd->insert({{"add", C_ARITHMETIC}, {"sub", C_ARITHMETIC}, {"neg", C_ARITHMETIC}, {"eq", C_ARITHMETIC}, {"gt", C_ARITHMETIC}, {"lt", C_ARITHMETIC}, {"and", C_ARITHMETIC}, {"or", C_ARITHMETIC}, {"not", C_ARITHMETIC}});
	str2cmd->insert({{"pop", C_POP}, {"push", C_PUSH}});
	str2cmd->insert({{"label", C_LABEL}, {"goto", C_GOTO}, {"if-goto", C_IF}});
	str2cmd->insert({{"function", C_FUNCTION}, {"call", C_CALL}, {"return", C_RETURN}});
}

bool Parser::initFileSuccess()
{
	return (ifs.is_open()) ? true : false;
}

Parser::~Parser()
{
	if (ifs.is_open())
		ifs.close();
	delete str2cmd;
}

bool Parser::hasMoreCommands()
{
	return !(ifs.eof());
}

void Parser::advance()
{
	const int command_least_length = 3;
	string str = "";
	getline(ifs, str);
	int split_pos = str.find("//");
	if (split_pos == 0 || str.size() < command_least_length)
		str = ""; 
	else if (split_pos > 0)
	{
		str = str.substr(0, split_pos);
		int end_pos = str.size();
		while (isspace(str[end_pos-1])) end_pos--;
		str = str.substr(0, end_pos);
	}
	current_command = str;
}

Command Parser::str2Cmd(string cmd)
{
	//cout << "in str2Cmd: " << cmd << endl;
	//cout << "length: " << cmd.size() << endl;
	unordered_map<string, Command>::iterator iter = str2cmd->find(cmd);
	if (iter != str2cmd->end())
		return iter->second;
	else
		return C_NOT;
}

Command Parser::commandType()
{
	string cmd = "";
	for (int i = 0; i < current_command.size(); i++)
	{
		if (isspace(current_command[i])) break;
		cmd += current_command[i];
	}
	//cout << "cmd: " << cmd << endl;
	return str2Cmd(cmd);
}

string Parser::arg1()
{
	if (commandType() == C_RETURN) throw "The funciton can not be called by return command!";
	if (commandType() == C_ARITHMETIC) return current_command;
	int pos_start = current_command.find(' ');
	int pos_end = pos_start;
	if (pos_start != string::npos)
		pos_end = current_command.find(' ', pos_start+1);
	int length = pos_end - pos_start-1;
	return current_command.substr(pos_start+1, length);
}

int Parser::arg2()
{
	if (commandType() != C_PUSH && commandType() != C_POP && commandType() != C_FUNCTION && commandType() != C_CALL) throw "This command can not call the function!";
	int pos_start = current_command.rfind(' ');
	return atoi(current_command.substr(pos_start+1).c_str());
}

//test
string Parser::getCommand()
{
	return current_command;
}

string Parser::cmd2Str(Command cmd)
{
	switch (cmd)
	{
		case C_ARITHMETIC: return "C_ARITHMETIC";
		case C_PUSH: return "C_PUSH";
		case C_POP: return "C_POP";
		case C_LABEL: return "C_LABEL";
		case C_GOTO: return "C_GOTO";
		case C_IF: return "C_IF";
		case C_FUNCTION: return "C_FUNCTION";
		case C_RETURN: return "C_RETURN";
		case C_CALL: return "C_CALL";
		case C_NOT: return "C_NOT";
	}
}
