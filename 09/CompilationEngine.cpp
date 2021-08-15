#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
		token.advance();
		cout << token.type2Str(token.tokenType()) << endl;
		//cout << token.getWord() << endl; 
	}
	return 0;
}
