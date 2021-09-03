#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens, int val)
	: compileByToken{new unordered_map<string, CompileType>}, compileByTag{new unordered_map<string, TagType>}, ops{new unordered_set<string>}, indentationSize{val}
{
	this->tokens = tokens;
	compileByToken->insert({{"static", CLASSVARDEC}, {"field", CLASSVARDEC}, {"constructor", SUBROUTINEDEC}, {"function", SUBROUTINEDEC}, {"method", SUBROUTINEDEC}, {"var", VAR}, {"if", IF}, {"while", WHILE}, {"do", DO}, {"return", RETURN}, {"let", LET}});
	compileByTag->insert({{"keyword", T_KEYWORD}, {"symbol", T_SYMBOL}, {"identifier", T_IDENTIFIER}, {"int_const", T_INI_CONST}, {"string_const", T_STRING_CONST}});
	ops->insert({"+", "-", "*", "/", "&", "|", "<", ">", "="});
	CompileClass(cout, 0);
}

CompilationEngine::~CompilationEngine()
{
	delete compileByToken;
}

void CompilationEngine::printTokenInXml(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<" << tokens.front().first << ">" ;
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

TagType CompilationEngine::tag2Type(string tag)
{
	unordered_map<string, TagType>::iterator iter = compileByTag->find(tag);
	if (iter == compileByTag->end()) return TagType::T_NONE;
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
	os << setw(indentation) << " " << "<class>" << endl;
	advanceUntilFlag(os, "{", indentation+indentationSize);
	bool loopFlag = true;
	while (loopFlag)
	{
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
	os << setw(indentation) << " "  << "</class>" << endl;
}

void CompilationEngine::CompileClassVarDec(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<classVarDec>" << endl;
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</classVarDec>" << endl;
}

void CompilationEngine::CompileSubroutineDec(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<subroutineDec>" << endl;
	advanceUntilFlag(os, "(", indentation+indentationSize);
	CompileParameterList(os, indentation+indentationSize);
	advanceUntilFlag(os, ")", indentation+indentationSize);
	CompileSubroutineBody(os, indentation+indentationSize);
	os << setw(indentation) << " " << "</subroutineDec>" << endl;
}

void CompilationEngine::CompileSubroutineBody(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<subroutineBody>" << endl;
	advanceUntilFlag(os, "{", indentation+indentationSize);
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
	advanceUntilFlag(os, "}", indentation+indentationSize);
	os << setw(indentation) << " " << "</subroutineBody>" << endl;
}

void CompilationEngine::CompileParameterList(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<parameterList>";
	advanceBeforeFlag(os, ")", indentation);
	os << " " << "</parameterList>" << endl;
}

void CompilationEngine::CompileVarDec(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<varDec>" << endl;
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</varDec>" << endl;
}

void CompilationEngine::CompileStatements(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<statements>" << endl;
	bool loopFlag = true;
	while (loopFlag)
	{
		switch (token2Type(tokens.front().second))
		{
				case CompileType::LET:
					CompileLet(os, indentation+indentationSize);
					break;
				case CompileType::IF:
					CompileIf(os, indentation+indentationSize);
					break;
				case CompileType::WHILE:
					CompileWhile(os, indentation+indentationSize);
					break;
				case CompileType::DO:
					CompileDo(os, indentation+indentationSize);
					break;
				case CompileType::RETURN:
					CompileReturn(os, indentation+indentationSize);
					break;
				default:
					loopFlag = false;
					break;
		}
	}
	os << setw(indentation) << " " << "</statements>" << endl;
}

void CompilationEngine::CompileLet(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<letStatement>" << endl;
	// output keyword and varname
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();

	if (tokens.front().second == "[")
	{
		advanceUntilFlag(os, "[", indentation+indentationSize);
		CompileExpression(os, indentation+indentationSize);
		advanceUntilFlag(os, "]", indentation+indentationSize);
	}
	advanceUntilFlag(os, "=", indentation+indentationSize);
	CompileExpression(os, indentation+indentationSize);
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</letStatement>" << endl;
}

void CompilationEngine::CompileWhile(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<whileStatement>" << endl;
	advanceUntilFlag(os, "(", indentation+indentationSize);
	CompileExpression(os, indentation+indentationSize);
	advanceUntilFlag(os, "{", indentation+indentationSize);
	CompileStatements(os, indentation+indentationSize);
	advanceUntilFlag(os, "}", indentation+indentationSize);
	os << setw(indentation) << " " << "</whileStatement>" << endl;
}

void CompilationEngine::CompileDo(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<doStatement>" << endl;
	//print keyword
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();
	CompileSubroutineCall(os, indentation+indentationSize);	
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</doStatement>" << endl;
}

void CompilationEngine::CompileReturn(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<returnStatement>" << endl;
	// print keyword
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();
	if (tokens.front().second != ";")
		CompileExpression(os, indentation+indentationSize);
	// print ; 
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();
	os << setw(indentation) << " " << "</returnStatement>" << endl;
}

void CompilationEngine::CompileIf(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<ifStatement>" << endl;
	//print keyword
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();

	// if branch operate
	advanceUntilFlag(os, "(", indentation+indentationSize);
	CompileExpression(os, indentation+indentationSize);
	advanceUntilFlag(os, "{", indentation+indentationSize);
	CompileStatements(os, indentation+indentationSize);
	advanceUntilFlag(os, "}", indentation+indentationSize);

	// else branch operate
	if (tokens.front().second == "else")
	{
		//print keyword
		printTokenInXml(os, indentation+indentationSize);
		tokens.pop();
		advanceUntilFlag(os, "{", indentation+indentationSize);
		CompileStatements(os, indentation+indentationSize);
		advanceUntilFlag(os, "}", indentation+indentationSize);
	}

	os << setw(indentation) << " " << "</ifStatement>" << endl;
}

void CompilationEngine::CompileExpression(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<expression>" << endl;
	CompileTerm(os, indentation+indentationSize);
	while (ops->find(tokens.front().second) != ops->end())
	{
		printTokenInXml(os, indentation+indentationSize);
		tokens.pop();
		CompileTerm(os, indentation+indentationSize);
	}
	os << setw(indentation) << " " << "</expression>" << endl;
}

void CompilationEngine::CompileTerm(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<term>" << endl;
	switch(tag2Type(tokens.front().first))
	{
		case T_KEYWORD:
		case T_INI_CONST:
		case T_STRING_CONST:
			printTokenInXml(os, indentation+indentationSize);
			tokens.pop();
			break;
		case T_IDENTIFIER:
			printTokenInXml(os, indentation+indentationSize);
			tokens.pop();
			if (tokens.front().second == "[")
			{
				advanceUntilFlag(os, "[", indentation+indentationSize);
				CompileExpression(os, indentation+indentationSize);
				advanceUntilFlag(os, "]", indentation+indentationSize);
			}
			else if (tokens.front().second == "(")
			{
				advanceUntilFlag(os, "(", indentation+indentationSize);
				CompileExpressionList(os, indentation+indentationSize);
				advanceUntilFlag(os, ")", indentation+indentationSize);
			}
			else if (tokens.front().second == ".")
			{
				CompileSubroutineCall(os, indentation+indentationSize);
			}
			break;
		case T_SYMBOL: 
			if (tokens.front().second == "(")
			{
				advanceUntilFlag(os, "(", indentation+indentationSize);
				CompileExpression(os, indentation+indentationSize);
				advanceUntilFlag(os, ")", indentation+indentationSize);
			}
			else if (tokens.front().second == "-" || tokens.front().second == "~")
			{
				printTokenInXml(os, indentation+indentationSize);
				tokens.pop();
				CompileTerm(os, indentation+indentationSize);
			}
		default:
			break;
	}
	os << setw(indentation) << " " << "</term>" << endl;
}

void CompilationEngine::CompileSubroutineCall(ostream& os, int indentation)
{
	printTokenInXml(os, indentation);
	tokens.pop();
	if (tokens.front().second == ".")
	{
		advanceUntilFlag(os, ".", indentation);
		printTokenInXml(os, indentation);
		tokens.pop();
	}
	advanceUntilFlag(os, "(", indentation);
	CompileExpressionList(os, indentation);
	advanceUntilFlag(os, ")", indentation);
}

void CompilationEngine::CompileExpressionList(ostream& os, int indentation) 
{
	os << setw(indentation) << " " << "<expressionList>";
	if (tokens.front().second == ")") 
	{
		os << " " << "</expressionList>" << endl;
		return;
	}
	CompileExpression(os, indentation+indentationSize);
	while (tokens.front().second == ",")
	{
		advanceUntilFlag(os, ",", indentation+indentationSize);	
		CompileExpression(os, indentation+indentationSize);
	}
	os << " " << "</expressionList>" << endl;
}
