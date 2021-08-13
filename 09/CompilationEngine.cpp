#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (true)
	{
		if (!token.hasMoreTokens()) break;
		token.advance();
		//cout << token.getWord() << endl; 
	}
	return 0;
}
