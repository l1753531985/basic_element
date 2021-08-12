#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
		cout << "in while branch in main!!!!!" << endl;
		//token.advance();
		//cout << token.getWord() << endl; 
	}
	return 0;
}
