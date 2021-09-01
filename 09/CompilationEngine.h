#include <iostream>
#include <fstream>
#include <queue> 
#include <unordered_map>

using namespace std;

enum CompileType {CLASSVARDEC, SUBROUTINEDEC, VAR, LET, IF, WHILE, DO, RETURN, NONE};

class CompilationEngine {
private:
	queue<pair<string, string>> tokens;
	unordered_map<string, CompileType>* compileByToken;
	void advanceUntilFlag(ostream&, string);
	void advanceBeforeFlag(ostream&, string);
	void printTokenInXml(ostream&);
	CompileType token2Type(string);
	// for test
	void popUntilFlag(string);
	// for test
	void popBeforeFlag(string);
public:
	CompilationEngine(const queue<pair<string, string>>&);
	~CompilationEngine();
	void CompileClass(ostream&);
	void CompileClassVarDec(ostream&);
	void CompileSubroutineDec(ostream&);
	void CompileParameterList(ostream&);
	void CompileVarDec(ostream&);
	void CompileStatements(ostream&);
	//fot test
	void printAllTokens();
};
