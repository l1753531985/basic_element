#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens, int val, string name, SymbolTable& st, VMWriter& vmw, bool writeFlag)
	: compileByToken{new unordered_map<string, CompileType>}, compileByTag{new unordered_map<string, TagType>}, ops{new unordered_set<string>}, indentationSize{val}, filename{name}, symbolTable{st}, vmwrite{vmw}, vmWriteFlag{writeFlag} 
{
	this->tokens = tokens;

	compileByToken->insert({{"static", C_CLASSVARDEC}, {"field", C_CLASSVARDEC}, {"constructor", C_SUBROUTINEDEC}, {"function", C_SUBROUTINEDEC}, {"method", C_SUBROUTINEDEC}, {"var", C_VAR}, {"if", C_IF}, {"while", C_WHILE}, {"do", C_DO}, {"return", C_RETURN}, {"let", C_LET}});
	compileByTag->insert({{"keyword", T_KEYWORD}, {"symbol", T_SYMBOL}, {"identifier", T_IDENTIFIER}, {"integerConstant", T_INI_CONST}, {"stringConstant", T_STRING_CONST}});
	ops->insert({"+", "-", "*", "/", "&amp;", "|", "&lt;", "&gt;", "="});

	if (vmWriteFlag) vmwrite.Constructor();

	ofile.open(filename);
	CompileClass(ofile, 0);
	// for test
	//CompileClass(cout, 0);
}

CompilationEngine::~CompilationEngine()
{
	if (ofile.is_open()) ofile.close();
	delete compileByToken;
	delete compileByTag;
	delete ops;
	if (vmWriteFlag) vmwrite.close();
}

string CompilationEngine::printTokenInXml(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<" << tokens.front().first << ">" ;
	os << " " << tokens.front().second << " ";
	os << "</" << tokens.front().first << ">" << endl;
	return tokens.front().second;
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
	if (iter == compileByToken->end()) return CompileType::C_NONE;
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

//for test
void CompilationEngine::printSymbolsTables(ostream& os)
{
	symbolTable.printAllElem(os);
}

void CompilationEngine::CompileClass(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<class>" << endl;
	// pop keyword
	printTokenInXml(os, indentation+indentationSize);
	tokens.pop();
	// get class name
	string className = tokens.front().second;
	advanceUntilFlag(os, "{", indentation+indentationSize);
	bool loopFlag = true;
	while (loopFlag)
	{
		switch (token2Type(tokens.front().second))
		{
			case C_CLASSVARDEC:
				CompileClassVarDec(os, indentation+indentationSize);
				break;
			case C_SUBROUTINEDEC:
				CompileSubroutineDec(os, indentation+indentationSize, className);
				break;
			case C_NONE:
				loopFlag = false;
				break;
			default:
				break;
		}
	}
	advanceUntilFlag(os, "}", indentation+indentationSize);
	os << setw(indentation) << " "  << "</class>" << endl;

	//fot test
	//printSymbolsTables(os);
}


void CompilationEngine::CompileClassVarDec(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<classVarDec>" << endl;
	if (!vmWriteFlag) getSymbolsFromDec(os, indentation+indentationSize);
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</classVarDec>" << endl;
}

void CompilationEngine::CompileSubroutineDec(ostream& os, int indentation, string className)
{
	os << setw(indentation) << " " << "<subroutineDec>" << endl;
	// pop keyword
	printTokenInXml(os, indentation+indentationSize); 
	tokens.pop();
	// pop return type
	printTokenInXml(os, indentation+indentationSize); 
	tokens.pop();
	// get function name
	string functionName = className + "." + tokens.front().second;
	// create code
	if (vmWriteFlag) vmwrite.writeFunction(functionName, symbolTable.varCount(KindType::VAR));
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
			case CompileType::C_VAR:
				CompileVarDec(os, indentation+indentationSize);
				break;
			case CompileType::C_LET:
			case CompileType::C_IF:
			case CompileType::C_WHILE:
			case CompileType::C_DO:
			case CompileType::C_RETURN:
				CompileStatements(os, indentation+indentationSize);
				break;
			case CompileType::C_NONE:
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
	os << setw(indentation) << " " << "<parameterList>" << endl;
	if (!vmWriteFlag) getSymbolsFromParaList(os, indentation);
}

void CompilationEngine::CompileVarDec(ostream& os, int indentation)
{
	os << setw(indentation) << " " << "<varDec>" << endl;
	if (!vmWriteFlag) getSymbolsFromDec(os, indentation+indentationSize);
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
				case CompileType::C_LET:
					CompileLet(os, indentation+indentationSize);
					break;
				case CompileType::C_IF:
					CompileIf(os, indentation+indentationSize);
					break;
				case CompileType::C_WHILE:
					CompileWhile(os, indentation+indentationSize);
					break;
				case CompileType::C_DO:
					CompileDo(os, indentation+indentationSize);
					break;
				case CompileType::C_RETURN:
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
	string name = tokens.front().second;
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

	// print className and . 
	string subrountineName = "";
	for (int i = 0; i < 2; i++)
	{
		subrountineName += printTokenInXml(os, indentation+indentationSize);
		tokens.pop();
	}
	subrountineName += tokens.front().second;
	int nargs = CompileSubroutineCall(os, indentation+indentationSize);	
	advanceUntilFlag(os, ";", indentation+indentationSize);
	os << setw(indentation) << " " << "</doStatement>" << endl;
	if (vmWriteFlag) vmwrite.writeCall(subrountineName, nargs);
	// cout << "name: " << subrountineName << "\targs: " << nargs << endl;
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
	string temp = "";
	switch(tag2Type(tokens.front().first))
	{
		case T_INI_CONST:
			if (vmWriteFlag) vmwrite.writePush(Segment::S_CONST, atoi(tokens.front().second.c_str()));
		case T_KEYWORD:
		case T_STRING_CONST:
			printTokenInXml(os, indentation+indentationSize);
			tokens.pop();
			break;
		case T_IDENTIFIER:
			temp += printTokenInXml(os, indentation+indentationSize);
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
				// print char .
				temp += printTokenInXml(os, indentation+indentationSize);
				tokens.pop();
				// print subroutine name
				temp += tokens.front().second;
				int nargs = CompileSubroutineCall(os, indentation+indentationSize);
				//cout << "name: " << temp << "\targs: " << nargs << endl;
				if (vmWriteFlag) vmwrite.writeCall(temp, nargs);
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

int CompilationEngine::CompileSubroutineCall(ostream& os, int indentation)
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
	int nargs = CompileExpressionList(os, indentation);
	advanceUntilFlag(os, ")", indentation);
	return nargs;
}

int CompilationEngine::CompileExpressionList(ostream& os, int indentation) 
{
	int count = 0;
	os << setw(indentation) << " " << "<expressionList>" << endl;
	if (tokens.front().second == ")") 
	{
		os << setw(indentation) << " " << "</expressionList>" << endl;
		return count;
	}
	CompileExpression(os, indentation+indentationSize);
	while (tokens.front().second == ",")
	{
		advanceUntilFlag(os, ",", indentation+indentationSize);	
		CompileExpression(os, indentation+indentationSize);
		count++;
	}
	os << setw(indentation) << " " << "</expressionList>" << endl;
	// at least has one expression
	return count+1;
}

void CompilationEngine::getSymbolsFromDec(ostream& os, int indentation)
{
	// get identifier kind
	printTokenInXml(os, indentation);
	string kind = tokens.front().second; 
	tokens.pop();

	// get identifier type 
	printTokenInXml(os, indentation);
	string type = tokens.front().second;
	tokens.pop();

	while (tokens.front().second != ";")
	{
		if (tokens.front().second == ",")
			advanceUntilFlag(os, ",", indentation);

		// get identifier itself 
		printTokenInXml(os, indentation);
		string symbolName = tokens.front().second;
		tokens.pop();
		
		symbolTable.Define(symbolName, type, symbolTable.str2Kind(kind));
	}
}

void CompilationEngine::getSymbolsFromParaList(ostream& os, int indentation)
{
	string kind = "argument";
	while (tokens.front().second != ")")
	{
		if (tokens.front().second == ",")
			advanceUntilFlag(os, ",", indentation);

		// get identifier type 
		printTokenInXml(os, indentation);
		string type = tokens.front().second;
		tokens.pop();

		// get identifier itself 
		printTokenInXml(os, indentation);
		string symbolName = tokens.front().second;
		tokens.pop();

		symbolTable.Define(symbolName, type, symbolTable.str2Kind(kind));
	}
}



