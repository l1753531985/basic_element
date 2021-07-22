#include <iostream>
#include "Parser.h"
#include "CodeWriter.h"

using namespace std;

int main(int ac, char* argv[])
{
	const int remain_arg_num = 2;
	string postfix = ".vm";
	CodeWriter code_write{argv[remain_arg_num-1]};
	code_write.writeInit();
	while (ac > remain_arg_num)
	{
		ac--;
		string file_name = argv[ac]; 
		string file_vm_name = file_name;
		string file_barely_name = file_name.substr(0, file_name.find(postfix));
		Parser parser{file_vm_name};
		while (true)
		{
			if (!parser.hasMoreCommands()) break;
			parser.advance();
			//cout << parser.getCommand() << endl;
			try {
				Command cmd = parser.commandType();
				//cout << parser.cmd2Str(cmd) << endl;
				switch (cmd)
				{
					case C_ARITHMETIC:
						code_write.writeArithmetic(parser.arg1());
						break;
					case C_PUSH: case C_POP:
						code_write.writePushPop(cmd, parser.arg1(), parser.arg2());
						break;
					case C_LABEL: case C_GOTO: case C_IF: code_write.writeControl(cmd, parser.arg1());
						break;
					case C_FUNCTION:
						code_write.writeFunction(parser.arg1(), parser.arg2());
						break;
					case C_RETURN:
						code_write.writeRuturn();
						break;
					case C_CALL:
						code_write.writeCall(parser.arg1(), parser.arg2());
						break;
					default:
						break;
				}
			} catch (const char* msg) {
				cerr << msg << endl;
			}
			//cout << parser.arg2().size() << endl;
			//cout << parser.cmd2Str(parser.commandType()) << endl;
		}
	}
	return 0;
}
