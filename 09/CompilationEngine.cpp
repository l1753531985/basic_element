#include "JackTokenizer.h"

template <typename stream>
void process(stream& stm)
{
	JackTokenizer token{"./Main.jack"};
	stm << "<token>" << endl;
	while (token.hasMoreTokens())
	{
		token.advance();
		switch (token.tokenType())
		{
			case TokenType::KEYWORD:
				stm << "<keyword> " << token.keyword() << " </keyword>" << endl;
				break;
			case TokenType::SYMBOL:
				stm << "<symbol> " << token.symbol() << " </symbol>" << endl;
				break;
			case TokenType::IDENTIFIER:
				stm << "<identifier> " << token.identifier() << " </identifier>" << endl;
				break;
			case TokenType::INT_CONST:
				stm << "<integerConstant> " << token.intVal() << " </integerConstant>" << endl;
				break;
			case TokenType::STRING_CONST:
				stm << "<stringConstant> " << token.stringVal() << " </stringConstant>" << endl;
				break;
			default:
				stm << "No any token type" << endl;
				break;

		}
		//stm << token.type2Str(token.tokenType()) << endl;
		//stm << token.getWord() << endl; 
	}
	stm << "</token>" << endl;
}

int main()
{
	ofstream ofile{"/root/basic_elements_projects/projects/09/test.xml"};
	process(ofile);
	return 0;
}
