#include "JackTokenizer.h"

int main()
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
		token.advance();
		switch (token.tokenType())
		{
			case TokenType::KEYWORD:
				cout << "<keyword> " << token.keyword() << " </keyword>" << endl;
				break;
			case TokenType::SYMBOL:
				cout << "<symbol> " << token.symbol() << " </symbol>" << endl;
				break;
			case TokenType::IDENTIFIER:
				cout << "<identifier> " << token.identifier() << " </identifier>" << endl;
				break;
			case TokenType::INT_CONST:
				cout << "<integerConstant> " << token.intVal() << " </integerConstant>" << endl;
				break;
			case TokenType::STRING_CONST:
				cout << "<stringConstant> " << token.stringVal() << " </stringConstant>" << endl;
				break;
			default:
				cout << "No any token type" << endl;
				break;

		}
		//cout << token.type2Str(token.tokenType()) << endl;
		//cout << token.getWord() << endl; 
	}
	return 0;
}
