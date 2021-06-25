#include <iostream>
#include "Parser.h"

using namespace std;

int main()
{
	Parser parser{"./test.txt"};
	while (true)
	{
		if (!parser.hasMoreCommands()) break;
		parser.advance();
		cout << parser.getCommand() << endl;
	}
	return 0;
}
