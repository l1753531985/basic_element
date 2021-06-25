#include <string>
#include <fstream>
#include <bitset>
#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

void perOperate(Parser& pas, SymbolTable& sym)
{
	int command_num = -1; 
	while (true)
	{
		pas.advance();
                if (!pas.hasMoreCommands()) break;
		//cout << parser.get_current_command() << endl;
		if (pas.commandType() == Command::A_COMMAND || pas.commandType() == Command::C_COMMAND) 
			command_num++;
		else if (pas.commandType() == Command::L_COMMAND)
		{
			string tmp = pas.symbol();
			string str = "";
			for (char x : tmp)
				if (!isspace(x))
					str += x;
			sym.addEntry(str, command_num+1);
		}
	}
}

void nomalOperate(Parser& parser, SymbolTable& symbols, Code& code, ofstream& ofs)
{
	//symbols.showTable();
	int command_num = 0;
	while (true)
	{
		parser.advance();
                if (!parser.hasMoreCommands()) break;
		//cout << parser.get_current_command() << endl;
		if (parser.commandType() == Command::A_COMMAND) 
		{
			command_num++;
			bool hasAlpha = false;
			bool isVariable = false;

			string tmp = parser.symbol();
			string str = "";
			for (char x : tmp)
				if (!isspace(x))
					str += x;
			for (char x : str)
				if (isalpha(x)) 
					hasAlpha = true;
			if (hasAlpha)
				isVariable = !symbols.contains(str);
			if (isVariable)
				symbols.addEntry(str);

			long long unsigned int num = 0;
			if (hasAlpha)
				num = symbols.getAddress(str);
			else 
				num = atoi(str.c_str());
			bitset<16> A_Command2bit{num};
			A_Command2bit.reset(15);
			ofs << A_Command2bit << endl;	
		}
		else if (parser.commandType() == Command::C_COMMAND) 
		{
			command_num++;
			bitset<7> comp = code.comp2bit(parser.comp());
			bitset<3> jump = code.jump2bit(parser.jump());
			bitset<3> dest = code.dest2bit(parser.dest());
			string cmd = comp.to_string() + dest.to_string() + jump.to_string();
			bitset<16> C_Command2bit{cmd};
			C_Command2bit.set(15);
			C_Command2bit.set(14);
			C_Command2bit.set(13);
			ofs << C_Command2bit<<endl;
		}
	}
	//symbols.showTable();
}

int main(int ac, char* argv[])
{
	if (ac == 1) return 0;
	string postfix = ".asm";
	string replace_postfix = ".hack";
	while (--ac) 
	{
		string file_name = argv[ac];
		string file_hack_name = file_name.substr(0, file_name.find(postfix)) + replace_postfix;
		//cout << file_hack_name << endl;
		ofstream ofs{file_hack_name};
		Code code;
		SymbolTable symbols;
		Parser parser{file_name};
		if (!parser.initFileSuccess()) return -1;
		// pre operation
		perOperate(parser, symbols);
		// nomal operation
		Parser parser2{file_name};
		nomalOperate(parser2, symbols, code, ofs);
		ofs.close();
	}
	return 0;
}
