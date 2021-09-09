#include <iostream>
#include <fstream>
#include <queue> 
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <unistd.h>
#include <fstream>

using namespace std;

enum CompileType {C_CLASSVARDEC, C_SUBROUTINEDEC, C_VAR, C_LET, C_IF, C_WHILE, C_DO, C_RETURN, C_NONE};

enum TagType {T_KEYWORD, T_INI_CONST, T_STRING_CONST, T_IDENTIFIER, T_SYMBOL, T_NONE};

class CompilationEngine {
private:
	queue<pair<string, string>> tokens;
	unordered_map<string, CompileType>* compileByToken;
	unordered_map<string, TagType>* compileByTag;
	unordered_set<string>* ops;
	void CompileSubroutineCall(ostream&, int);
	void advanceUntilFlag(ostream&, string, int);
	void advanceBeforeFlag(ostream&, string, int);
	void printTokenInXml(ostream&, int);
	CompileType token2Type(string);
	TagType tag2Type(string);
	// for test
	void popUntilFlag(string);
	// for test
	void popBeforeFlag(string);
	int indentationSize;
	string filename;
	ofstream ofile;
	unordered_map<string, pair<string, string>> symbolsInTables;
	void getSymbolsFromDec(ostream&, int);
	void getSymbolsFromParaList(ostream&, int);
public:
	CompilationEngine(const queue<pair<string, string>>&, int, string);
	~CompilationEngine();
	void CompileClass(ostream&, int);
	void CompileClassVarDec(ostream&, int);
	void CompileSubroutineDec(ostream&, int);
	void CompileSubroutineBody(ostream&, int);
	void CompileParameterList(ostream&, int);
	void CompileVarDec(ostream&, int);
	void CompileStatements(ostream&, int);
	void CompileLet(ostream&, int);
	void CompileDo(ostream&, int);
	void CompileWhile(ostream&, int);
	void CompileReturn(ostream&, int);
	void CompileIf(ostream&, int);
	void CompileExpression(ostream&, int);
	void CompileTerm(ostream&, int); 
	void CompileExpressionList(ostream&, int);
	unordered_map<string, pair<string, string>>* getSymbolsTable();
	//fot test
	void printAllTokens();
	void printSymbolsTables(ostream&, unordered_map<string, pair<string, string>>*);
};
