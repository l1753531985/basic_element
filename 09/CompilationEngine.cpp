#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
		token.advance();
		//cout << token.getWord() << endl; 
	}
	return 0;
}
