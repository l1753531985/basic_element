#include "Parser.h"
#include <iostream>

using namespace std;

Parser::Parser():current_command{""} {}

Parser::Parser(string file)
	:current_command{""} 
{
	ifs.open(file);
}

bool Parser::initFileSuccess()
{
	return (ifs.is_open()) ? true : false;
}

Parser::~Parser()
{
	if (ifs.is_open())
		ifs.close();
}

bool Parser::hasMoreCommands()
{
	return !(ifs.eof());
}

void Parser::advance()
{
	string str = "";
	getline(ifs, str);
	current_command = str;
}

//test
string Parser::getCommand()
{
	return current_command;
}
