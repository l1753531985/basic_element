#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens, int val)
	: compileByToken{new unordered_map<string, CompileType>}, indentationSize{val}
{
	this->tokens = tokens;
	compileByToken->insert({{"static", CLASSVARDEC}, {"field", CLASSVARDEC}, {"constructor", SUBROUTINEDEC}, {"function", SUBROUTINEDEC}, {"method", SUBROUTINEDEC}, {"var", VAR}, {"if", IF}, {"while", WHILE}, {"do", DO}, {"return", RETURN}});
	CompileClass(cout, 0);
}

CompilationEngine::~CompilationEngine()
{
	delete compileByToken;
}

void CompilationEngine::printTokenInXml(ostream& os, int indentation)
{
	os << setw(indentation) << "<" << tokens.front().first << ">" ;
	os << " " << tokens.front().second << " ";
	os << "</" << tokens.front().first << ">" << endl;
}

// print all elements and flag
void CompilationEngine::advanceUntilFlag(ostream& os, string flag, int indentation)
{
	bool keepLoop = true;
	while (keepLoop)
	{
		printTokenInXml(os, indentation);
		if (tokens.front().second == flag) keepLoop = false;
		tokens.pop();
	}
}

// print all elements before flag
void CompilationEngine::advanceBeforeFlag(ostream& os, string flag, int indentation)
{
	while (tokens.front().second != flag)
	{
		printTokenInXml(os, indentation);
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

void CompilationEngine::CompileClass(ostream& os, int indentation)
{
	os << "<class>" << endl;
	advanceUntilFlag(os, "{", indentation+indentationSize);
	bool loopFlag = true;
	while (loopFlag)
	{
		sleep(1);
		switch (token2Type(tokens.front().second))
		{
			case CLASSVARDEC:
				CompileClassVarDec(os, indentation+indentationSize);
				break;
			case SUBROUTINEDEC:
				CompileSubroutineDec(os, indentation+indentationSize);
				break;
			case NONE:
				loopFlag = false;
				break;
			default:
				break;
		}
	}
	advanceUntilFlag(os, "}", indentation+indentationSize);
	os << "</class>" << endl;
}

void CompilationEngine::CompileClassVarDec(ostream& os, int indentation)
{
	cout << "indentation: " << indentation << endl;
	os << setw(indentation) << "<CompileClassVarDec>" << endl;
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << "</CompileClassVarDec>" << endl;
}

void CompilationEngine::CompileSubroutineDec(ostream& os, int indentation)
{
	os << setw(indentation) << "<CompileSubroutineDec>" << endl;
	advanceUntilFlag(os, "(", indentation);
	CompileParameterList(os, indentation+indentationSize);
	os << setw(indentation) << "<subroutineBody>" << endl;
	advanceUntilFlag(os, "{", indentation);
	bool loopFlag = true;
	while (loopFlag)
	{
		switch (token2Type(tokens.front().second))
		{
			case CompileType::VAR:
				CompileVarDec(os, indentation+indentationSize);
				break;
			case CompileType::LET:
			case CompileType::IF:
			case CompileType::WHILE:
			case CompileType::DO:
			case CompileType::RETURN:
				CompileStatements(os, indentation+indentationSize);
				break;
			case CompileType::NONE:
				loopFlag = false;
				break;
			default:
				break;
		}
	}
	advanceUntilFlag(os, "}", indentation);
	os << setw(indentation) << "</subroutineBody>" << endl;
	os << setw(indentation) << "</CompileSubroutineDec>" << endl;
}

void CompilationEngine::CompileParameterList(ostream& os, int indentation)
{
	os << setw(indentation) << "<parameterList>";
	advanceBeforeFlag(os, ")", indentation);
	os << "</parameterList>" << endl;
}

void CompilationEngine::CompileVarDec(ostream& os, int indentation)
{
	os << setw(indentation) << "<varDec>" << endl;
	advanceUntilFlag(os, ";", indentation);
	os << setw(indentation) << "</varDec>" << endl;
}

void CompilationEngine::CompileStatements(ostream& os, int indentation)
{
	os << setw(indentation) << "<CompileStatements>" << endl;
	os << setw(indentation) << "</CompileStatements>" << endl;
}
