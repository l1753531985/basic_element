#include <iostream>
#include <fstream>
#include <queue> 
#include <unordered_map>
#include <iomanip>
#include <unistd.h>

using namespace std;

enum CompileType {CLASSVARDEC, SUBROUTINEDEC, VAR, LET, IF, WHILE, DO, RETURN, NONE};

class CompilationEngine {
private:
	queue<pair<string, string>> tokens;
	unordered_map<string, CompileType>* compileByToken;
	void advanceUntilFlag(ostream&, string, int);
	void advanceBeforeFlag(ostream&, string, int);
	void printTokenInXml(ostream&, int);
	CompileType token2Type(string);
	// for test
	void popUntilFlag(string);
	// for test
	void popBeforeFlag(string);
	int indentationSize;
public:
	CompilationEngine(const queue<pair<string, string>>&, int);
	~CompilationEngine();
	void CompileClass(ostream&, int);
	void CompileClassVarDec(ostream&, int);
	void CompileSubroutineDec(ostream&, int);
	void CompileParameterList(ostream&, int);
	void CompileVarDec(ostream&, int);
	void CompileStatements(ostream&, int);
	//fot test
	void printAllTokens();
};
