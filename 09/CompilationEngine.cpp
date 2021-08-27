#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(const queue<pair<string, string>>& tokens)
	:keyword2Type{new unordered_map<string, CompilationType>}, tag2Type{new unordered_map<string, ExpressionType>}
{
	this->tokens = tokens;
	keyword2Type->insert({{"class", CompilationType::CLASS}, {"static", CompilationType::CLASSVARDEC}, {"field", CompilationType::CLASSVARDEC}, {"constructor", CompilationType::SUBROUTINEDEC}, {"function", CompilationType::SUBROUTINEDEC}, {"method", CompilationType::SUBROUTINEDEC}, {"var", CompilationType::VARDEC}, {"do", CompilationType::DO}, {"let", CompilationType::LET}, {"while", CompilationType::WHILE}, {"return", CompilationType::RETURN}, {"if", CompilationType::IF}});	
	tag2Type->insert({{"keyword", ExpressionType::E_KEYWORD}, {"symbol", ExpressionType::E_SYMBOL}, {"identifier", ExpressionType::E_IDENTIFIER}, {"integerConstant", ExpressionType::E_INT_CONST}, {"stringConstant", ExpressionType::E_STRING_CONST}});
	CompileClass(cout);
}

CompilationEngine::~CompilationEngine()
{
	delete keyword2Type;
}


void CompilationEngine::CompileClass(ostream& os)
{
	os << "<class>"<< endl;
	advanceUntilFlag(os, "{");
	switch (keyword2Type->find(tokens.front().second)->second)
	{
		case CompilationType::SUBROUTINEDEC:
			CompileSubroutine(os);
			break;
		case CompilationType::CLASSVARDEC:
			CompileClassVarDec(os);
			break;
		default:
			break;
	}
	advanceUntilFlag(os, "}");
	os << "</class>" << endl;	
}

void CompilationEngine::CompileClassVarDec(ostream& os)
{
	os << "<classVarDec>"<< endl;
	popUntilFlag(";");
	os << "</classVarDec>"<< endl;
}

void CompilationEngine::CompileSubroutine(ostream& os)
{
	os << "<subroutineDec>" << endl;
	advanceUntilFlag(os, "(");
	os << "<parameterList> ";
	CompileParameterList(os);
	os << "</parameterList>" << endl;	
	advanceUntilFlag(os, ")");
	os << "<subroutineBody>" << endl;
	advanceUntilFlag(os, "{");

	while (tokens.front().second != "}")
	{
		string keyword = tokens.front().second;
		// for test
		// os << "keyword: " << keyword << endl;
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
	advanceUntilFlag(os, "}");
	os << "</subroutineBody>" << endl;
	os << "</subroutineDec>" << endl;
}

// print all elements and flag
void CompilationEngine::advanceUntilFlag(ostream& os, string flag)
{
	bool keepLoop = true;
	while (keepLoop)
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		if (tokens.front().second == flag) keepLoop = false;
		tokens.pop();
	}
}

// print all elements before flag
void CompilationEngine::advanceBeforeFlag(ostream& os, string flag)
{
	while (tokens.front().second != ")")
	{
		os << "<" << tokens.front().first << ">";
		os << " " << tokens.front().second << " ";  	
		os << "</" << tokens.front().first << ">" << endl;
		tokens.pop();
	}
}

void CompilationEngine::CompileVarDec(ostream& os)
{
	os << "<varDec>" << endl;	
	// advanceUntilFlag(os, ";");
	popUntilFlag(";");
	os << "</varDec>" << endl;	
}

void CompilationEngine::CompileExpression(ostream& os)
{
	os << "<expression>" << endl;
	/*
	CompileTerm(os);
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();
	CompileTerm(os);
	*/
	os << "</expression>" << endl;
}

void CompilationEngine::CompileTerm(ostream& os)
{
	os << "<term>" << endl;
	ExpressionType termStart = tag2Type->find(tokens.front().first)->second;
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();
	string afterVarName = "";
	switch (termStart)
	{
		case ExpressionType::E_IDENTIFIER:
			afterVarName = tokens.front().second;
			if (afterVarName == ".")
			break;
		default:
			break;
	}
	os << "</term>" << endl;
}

void CompilationEngine::CompileStatements(ostream& os)
{
	os << "<statements>" << endl;
	bool keepLoop = true;
	while (keepLoop)
	{
		unordered_map<string, CompilationType>::iterator iter = keyword2Type->find(tokens.front().second); 
		CompilationType tmp = (iter == keyword2Type->end()) ? NONE : iter->second; 
		switch (tmp)
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
				keepLoop = false;
				break;
		}
	}
	os << "</statments>" << endl;
}

void CompilationEngine::CompileDo(ostream& os)
{
	os << "<doStatement>" << endl;
	// advanceUntilFlag(os, "(");
	popUntilFlag("(");
	CompileExpression(os);
	// advanceUntilFlag(os, ";");
	popUntilFlag(";");
	os << "</doStatement>" << endl;
}

void CompilationEngine::CompileLet(ostream& os)
{
	os << "<letStatement>" << endl;
	// output keyword let
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();
	// output identifier varName 
	os << "<" << tokens.front().first << ">";
	os << " " << tokens.front().second << " ";  	
	os << "</" << tokens.front().first << ">" << endl;
	tokens.pop();

	if (tokens.front().second == "[")
	{
		// advanceUntilFlag(os, "[");
		popUntilFlag("[");
		CompileExpression(os);
		// advanceUntilFlag(os, "]");
		popUntilFlag("]");
	}
	// advanceUntilFlag(os, "=");
	popUntilFlag("=");
	CompileExpression(os);
	// advanceUntilFlag(os, ";");
	popUntilFlag(";");
	os << "</letStatement>" << endl;
}

void CompilationEngine::CompileWhile(ostream& os)
{
	os << "<whileStatement>" << endl;
	// advanceUntilFlag(os, "(");
	popUntilFlag("(");
	CompileExpression(os);
	// advanceUntilFlag(os, "{");
	popUntilFlag("{");
	// CompileStatements(os);
	// advanceUntilFlag(os, "}");
	popUntilFlag("}");
	os << "</whileStatement>" << endl;
}

void CompilationEngine::CompileReturn(ostream& os)
{
	os << "<returnStatement>" << endl;
	// advanceUntilFlag(os, ";");
	popUntilFlag(";");
	os << "</returnStatement>" << endl;
}

void CompilationEngine::CompileIf(ostream& os)
{
	os << "<ifStatement>" << endl;
	os << "</ifStatement>" << endl;
}

void CompilationEngine::CompileParameterList(ostream& os)
{
	//advanceBeforeFlag(os, ")");
	popBeforeFlag(")");
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
