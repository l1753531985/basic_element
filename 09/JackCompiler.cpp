#include <queue>
#include <unordered_map>
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "VMWriter.h"

void getTokens(queue<pair<string, string>>& q, queue<string>& identifiers)
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
				identifiers.push(singleToken);
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
	queue<string> identifiersInOrder;
	getTokens(tokens, identifiersInOrder);
	
	int indentation = 4;
	// symbolTable is a data struct for store symbols
	SymbolTable st;
	CompilationEngine ce{tokens, indentation, "test.xml", st};
	// for test
	//ce.printAllTokens();
	//st.printAllElem(cout);
	
	VMWriter vmw{"test.vm"};
	return 0;
}
