#include "Parser.h"

Parser::Parser():current_command{""}{}

Parser::Parser(string file_name)
	:current_command{""}
{
	ifile.open(file_name);
}

bool Parser::initFileSuccess()
{
	return (ifile.is_open()) ? true : false;
}

Parser::~Parser()
{
	if (ifile.is_open())
		ifile.close();
}

bool Parser::hasMoreCommands()
{
	return !(ifile.eof());
}

bool Parser::commandIsEmpty()
{
	return current_command.empty();
}

void Parser::advance()
{
	string str = "";
	getline(ifile, str); 
	//cout << "string is: " << str << endl;

	// filiter note
	int note_pos = str.find('/');
	switch (note_pos)
	{
		case -1:
			break;
		case 0:
			str = "";
			break;
		default:
			str = str.substr(0, note_pos-1);
	}

	// cout << "after string is: " << str << endl;
	// cout << command2string(whichType(str)) << endl;
	if (whichType(str) == Command::NOT_A_COMMAND) str = "";
	if (!str.empty())
	{
		string tmp_str = "";
		for (char c : str)
			if (!isblank(c))
				tmp_str += c;
		current_command = tmp_str;
		//cout << "string is not empty" << endl;
	}
}

Command Parser::whichType(string str)
{
	//cout << "in whichType function" << endl;
	if (str == "") return NOT_A_COMMAND; 
	// check whether the command is A_COMMAND or L_COMMAND
	if (str.find('@')!= string::npos) return A_COMMAND;
	// check whether the command is C_COMMAND 
	if (str.find('=') !=  string::npos || str.find(';') != string::npos) return C_COMMAND;
	// check whether the command is L_COMMAND 
	if (str.find('(') !=  string::npos && str.find(')') != string::npos) return L_COMMAND;
	return NOT_A_COMMAND; 
}

Command Parser::commandType()
{
	return whichType(current_command);
}

string Parser::symbol()
{
	if (commandType() == Command::A_COMMAND) 
		return current_command.substr(current_command.find('@')+1);
	if (commandType() == Command::L_COMMAND) 
		return current_command.substr(current_command.find('(')+1, current_command.find(')')-1);
	return current_command;
}

string Parser::dest()
{
	//cout << "current_command: " << current_command << endl;
	if (commandType() == Command::C_COMMAND && current_command.find('=') != string::npos) 
	{
		//cout << "substr: " << current_command.substr(0, current_command.find("=")) << endl;
		return current_command.substr(0, current_command.find('='));
	}
	else
	{
		return "NULL";
	}
}

string Parser::comp()
{
	//cout << "current_command: " << current_command << endl;
	if (commandType() == Command::C_COMMAND && current_command.find('=') != string::npos) 
	{
		return current_command.substr(current_command.find('=')+1);
	}
	else if (commandType() == Command::C_COMMAND && current_command.find(';') != string::npos)
	{
		return current_command.substr(0, current_command.find(';'));
	}
	else
	{
		return "NULL";
	}
}

string Parser::jump()
{  
	//cout << "current_command: " << current_command << endl;
	if (commandType() == Command::C_COMMAND && current_command.find(';') != string::npos) 
	{
		return current_command.substr(current_command.find(';')+1);
	}
	else
	{
		return "NULL";
	}
}

// test
string Parser::command2string(Command cmd)
{
	if (cmd == Command::A_COMMAND) 
		return "A_COMMAND";
	else if (cmd == Command::C_COMMAND) 
		return "C_COMMAND";
	else if (cmd == Command::L_COMMAND) 
		return "L_COMMAND";
	else	
		return "NOT_A_COMMAND";
}


