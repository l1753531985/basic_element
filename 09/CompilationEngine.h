#include <iostream>
#include <fstream>
#include <queue> 
#include <unordered_map>

using namespace std;

enum CompilationType {CLASS, CLASSVARDEC, SUBROUTINEDEC, VARDEC, DO, LET, WHILE, RETURN, IF , EXPRESSION, TERM, EXPRESSIONLIST};

class CompilationEngine {
private:
	queue<pair<string, string>> tokens;
	unordered_map<string, CompilationType>* keyword2Type; 
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
	//test
	void printAllTokens();
};
