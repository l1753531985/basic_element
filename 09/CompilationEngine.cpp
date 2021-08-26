#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens)
	:keyword2Type{new unordered_map<string, CompilationType>}
{
	this->tokens = tokens;
	keyword2Type->insert({{"class", CompilationType::CLASS}, {"static", CompilationType::CLASSVARDEC}, {"field", CompilationType::CLASSVARDEC}, {"constructor", CompilationType::SUBROUTINEDEC}, {"function", CompilationType::SUBROUTINEDEC}, {"method", CompilationType::SUBROUTINEDEC}, {"var", CompilationType::VARDEC}, {"do", CompilationType::DO}, {"let", CompilationType::LET}, {"while", CompilationType::WHILE}, {"return", CompilationType::RETURN}, {"if", CompilationType::IF}});	
	CompileClass(cout);
}

CompilationEngine::~CompilationEngine()
{
}

void CompilationEngine::printAllTokens()
{
	while (!tokens.empty())
	{
		cout << tokens.front().first << ": " << tokens.front().second << endl;
		tokens.pop();
	}
}

void CompilationEngine::CompileClass(ostream& os)
{
	os << "<class>"<< endl;
	bool keepLoop = true;
	while (keepLoop)
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		if (tokens.front().second == "{") keepLoop = false;
		tokens.pop();
	}

	//scan next to the character '{'
	switch (keyword2Type->find(tokens.front().second)->second)
	{
		case CompilationType::SUBROUTINEDEC:
			CompileSubroutine(os);
			break;
		default:
			break;
	}

	// push character '}'
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();

	os << "</class>" << endl;	
}

void CompilationEngine::CompileSubroutine(ostream& os)
{
	os << "<subroutineDec>" << endl;
	bool keepLoop = true;
	while (keepLoop)
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		if (tokens.front().second == "(") keepLoop = false;
		tokens.pop();
	}
	os << "<parameterList> ";
	CompileParameterList(os);
	os << "</parameterList>" << endl;	

	// push character ')' 
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();

	os << "<subroutineBody>" << endl;

	// push character '{'
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();

	// give function from token
	while (tokens.front().second != "}")
	{
		string keyword = tokens.front().second;
		os << "keyword: " << keyword << endl;
		switch (keyword2Type->find(keyword)->second)
		{
			case CompilationType::VARDEC:
				CompileVarDec(os);
				break;
			case CompilationType::DO:
			case CompilationType::LET:
			case CompilationType::WHILE:
			case CompilationType::RETURN:
			case CompilationType::IF:
				CompileStatements(os);	
				break;
			default:
				break;
		}
	}
	os << "</subroutineBody>" << endl;
	os << "</subroutineDec>" << endl;
}

void CompilationEngine::CompileVarDec(ostream& os)
{
	os << "<varDec>" << endl;	
	bool keepLoop = true;
	while (keepLoop)
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		if (tokens.front().second == ";") keepLoop = false;
		tokens.pop();
	}
	os << "</varDec>" << endl;	
}

void CompilationEngine::CompileStatements(ostream& os)
{
	os << "<statements>" << endl;
	switch (keyword2Type->find(tokens.front().second)->second)
	{
		case CompilationType::DO:
			CompileDo(os);
			break;
		case CompilationType::LET:
			CompileLet(os);
			break;
		case CompilationType::WHILE:
			CompileWhile(os);
			break;
		case CompilationType::RETURN:
			CompileReturn(os);
			break;
		case CompilationType::IF:
			CompileIf(os);
			break;
		default:
			break;
	}
	os << "</statments>" << endl;
}

void CompilationEngine::CompileDo(ostream& os)
{
	os << "<doStatement>" << endl;
	os << "</doStatement>" << endl;
}

void CompilationEngine::CompileLet(ostream& os)
{
	os << "<letStatement>" << endl;
	os << "</letStatement>" << endl;
}

void CompilationEngine::CompileWhile(ostream& os)
{
	os << "<whileStatement>" << endl;
	os << "</whileStatement>" << endl;
}

void CompilationEngine::CompileReturn(ostream& os)
{
	os << "<returnStatement>" << endl;
	os << "</returnStatement>" << endl;
}

void CompilationEngine::CompileIf(ostream& os)
{
	os << "<ifStatement>" << endl;
	os << "</ifStatement>" << endl;
}

void CompilationEngine::CompileParameterList(ostream& os)
{
	while (tokens.front().second != ")")
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		tokens.pop();
	}
}
