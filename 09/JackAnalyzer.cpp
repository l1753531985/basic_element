#include <queue>
#include "JackTokenizer.h"
#include "CompilationEngine.h"

void process(queue<pair<string, string>>& q)
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
		//stm << token.getWord() << endl;
		token.advance();
		string tag = "", singleToken = "";
		switch (token.tokenType())
		{
			case TokenType::KEYWORD:
				tag = "keyword";	
				singleToken = token.keyword();
				break;
			case TokenType::SYMBOL:
				tag = "symbol";	
				singleToken = token.symbol();
				break;
			case TokenType::IDENTIFIER:
				tag = "identifier";	
				singleToken = token.identifier();
				break;
			case TokenType::INT_CONST:
				tag = "integerConstant";	
				singleToken = token.intVal();
				break;
			case TokenType::STRING_CONST:
				tag = "stringConstant";	
				singleToken = token.stringVal();
				break;
			default:
				tag = "";
				singleToken = "";
				break;

		}
		pair<string, string> p{tag, singleToken};
		q.push(p);
		//stm << token.type2Str(token.tokenType()) << endl;
		//stm << token.getWord() << endl; 
	}
}

int main()
{
	queue<pair<string, string>> tokens;
	process(tokens);
	CompilationEngine ce{tokens};
	//ce.printAllTokens();
	return 0;
}
