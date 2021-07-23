#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (true)
	{
		if (!token.hasMoreTokens()) break;
		token.advance();
		cout << token.getLine() << endl; 
	}
	return 0;
}
