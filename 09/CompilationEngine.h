#include <iostream>
#include <fstream>
#include <queue> 
#include <unordered_map>

using namespace std;

enum CompilationType {CLASS, CLASSVARDEC, SUBROUTINEDEC, VARDEC, DO, LET, WHILE, RETURN, IF, NONE};

enum ExpressionType {E_SYMBOL, E_KEYWORD, E_IDENTIFIER, E_INT_CONST, E_STRING_CONST};

class CompilationEngine {
private:
	queue<pair<string, string>> tokens;
	unordered_map<string, CompilationType>* keyword2Type; 
	unordered_map<string, ExpressionType>* tag2Type;
	void advanceUntilFlag(ostream&, string);
	void advanceBeforeFlag(ostream&, string);
	// for test
	void popUntilFlag(string);
	// for test
	void popBeforeFlag(string);
public:
	CompilationEngine(const queue<pair<string, string>>&);
	~CompilationEngine();
	void CompileClass(ostream&);
	void CompileSubroutine(ostream&); 
	void CompileParameterList(ostream&);
	void CompileVarDec(ostream&);
	void CompileStatements(ostream&);
	void CompileDo(ostream&);
	void CompileLet(ostream&);
	void CompileWhile(ostream&);
	void CompileReturn(ostream&);
	void CompileIf(ostream&);
	void CompileExpression(ostream&);
	void CompileTerm(ostream&);
	void CompileClassVarDec(ostream&);
	//fot test
	void printAllTokens();
};
