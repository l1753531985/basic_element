#include <iostream>
#include "Parser.h"

using namespace std;

int main()
{
	Parser parser{"./BasicTest.vm"};
	while (true)
	{
		if (!parser.hasMoreCommands()) break;
		parser.advance();
		//cout << parser.getCommand() << endl;
		try {
			//cout << parser.arg1() << endl;
			cout << parser.arg2() << endl;
		} catch (const char* msg) {
			cerr << msg << endl;
		}
		//cout << parser.arg2().size() << endl;
		//cout << parser.cmd2Str(parser.commandType()) << endl;
	}
	return 0;
}
