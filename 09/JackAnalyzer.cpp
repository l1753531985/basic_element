#include <queue>
#include <unordered_map>
#include "JackTokenizer.h"
#include "CompilationEngine.h"

void getTokens(queue<pair<string, string>>& q)
{
	JackTokenizer token{"./Main.jack"};
	while (token.hasMoreTokens())
	{
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
	}
}

int main()
{
	queue<pair<string, string>> tokens;
	getTokens(tokens);
	
	int indentation = 4;
	CompilationEngine ce{tokens, indentation, "test.xml"};
	//ce.printAllTokens();
	
	unordered_map<string, pair<string, string>>* symbolsOfTable{ce.getSymbolsTable()};
	ce.printSymbolsTables(cout, symbolsOfTable);
	delete symbolsOfTable;

	return 0;
}
