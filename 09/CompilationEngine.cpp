#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens)
	: compileByToken{new unordered_map<string, CompileType>}
{
	this->tokens = tokens;
	compileByToken->insert({{"static", CLASSVARDEC}, {"field", CLASSVARDEC}, {"constructor", SUBROUTINEDEC}, {"function", SUBROUTINEDEC}, {"method", SUBROUTINEDEC}, {"var", VAR}, {"if", IF}, {"while", WHILE}, {"do", DO}, {"return", RETURN}});
	CompileClass(cout);
}

CompilationEngine::~CompilationEngine()
{
	delete compileByToken;
}

void CompilationEngine::printTokenInXml(ostream& os)
{
	os << "<" << tokens.front().first << ">" ;
	os << " " << tokens.front().second << " ";
	os << "</" << tokens.front().first << ">" << endl;
}

// print all elements and flag
void CompilationEngine::advanceUntilFlag(ostream& os, string flag)
{
	bool keepLoop = true;
	while (keepLoop)
	{
		printTokenInXml(os);
		if (tokens.front().second == flag) keepLoop = false;
		tokens.pop();
	}
}

// print all elements before flag
void CompilationEngine::advanceBeforeFlag(ostream& os, string flag)
{
	while (tokens.front().second != flag)
	{
		printTokenInXml(os);
		tokens.pop();
	}
}

CompileType CompilationEngine::token2Type(string token)
{
	unordered_map<string, CompileType>::iterator iter = compileByToken->find(token);
	if (iter == compileByToken->end()) return CompileType::NONE;
	return iter->second;
}

//fot test
void CompilationEngine::printAllTokens()
{
	while (!tokens.empty())
	{
		cout << tokens.front().first << ": " << tokens.front().second << endl;
		tokens.pop();
	}
}

// for test
void CompilationEngine::popUntilFlag(string flag)
{
	bool keepLoop = true;
	while (keepLoop)
	{
		if (tokens.front().second == flag) keepLoop = false;
		tokens.pop();
	}
}

// for test
void CompilationEngine::popBeforeFlag(string flag)
{
	while (tokens.front().second != ")") tokens.pop();
}

void CompilationEngine::CompileClass(ostream& os)
{
	os << "<class>" << endl;
	advanceUntilFlag(os, "{");
	bool loopFlag = true;
	while (loopFlag)
	{
		switch (token2Type(tokens.front().second))
		{
			case CLASSVARDEC:
				CompileClassVarDec(os);
				break;
			case SUBROUTINEDEC:
				CompileSubroutineDec(os);
				break;
			case NONE:
				loopFlag = false;
				break;
			default:
				break;
		}
	}
	advanceUntilFlag(os, "}");
	os << "</class>" << endl;
}

void CompilationEngine::CompileClassVarDec(ostream& os)
{
	os << "<CompileClassVarDec>" << endl;
	advanceUntilFlag(os, ";");
	os << "</CompileClassVarDec>" << endl;
}

void CompilationEngine::CompileSubroutineDec(ostream& os)
{
	os << "<CompileSubroutineDec>" << endl;
	advanceUntilFlag(os, "(");
	CompileParameterList(os);
	os << "<subroutineBody>" << endl;
	advanceUntilFlag(os, "{");
	bool loopFlag = true;
	while (loopFlag)
	{
		switch (token2Type(tokens.front().second))
		{
			case CompileType::VAR:
				CompileVarDec(os);
				break;
			case CompileType::LET:
			case CompileType::IF:
			case CompileType::WHILE:
			case CompileType::DO:
			case CompileType::RETURN:
				CompileStatements(os);
				break;
			case CompileType::NONE:
				loopFlag = false;
				break;
			default:
				break;
		}
	}
	advanceUntilFlag(os, "}");
	os << "</subroutineBody>" << endl;
	os << "</CompileSubroutineDec>" << endl;
}

void CompilationEngine::CompileParameterList(ostream& os)
{
	os << "<parameterList>";
	advanceBeforeFlag(os, ")");
	os << "</parameterList>" << endl;
}

void CompilationEngine::CompileVarDec(ostream& os)
{
	os << "<varDec>" << endl;
	advanceUntilFlag(os, ";");
	os << "</varDec>" << endl;
}

void CompilationEngine::CompileStatements(ostream& os)
{
	os << "<CompileStatements>" << endl;
	os << "</CompileStatements>" << endl;
}
