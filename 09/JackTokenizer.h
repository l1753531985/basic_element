#ifndef JackTokenizer_H
#define JackTokenizer_H
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <queue>
#include <unistd.h>
#include <fstream>

using namespace std;

enum TokenType {KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST};

enum Status {STRSTART, STREND};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	string line;
	string token;
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
	queue<string>* lines;
	queue<string>* line2words;
	queue<string>* words;
	queue<string> stringBuffer;
	string str2Std(string);
	void getLineFromFile();
	void lineSplitIntoWords();
	void wordSplitIntoTokens();
	void nextToken();
	void token2StdChar();
public:
	JackTokenizer(string);
	bool hasMoreTokens();
	void advance();
	TokenType tokenType();
	string keyword();
	string symbol();
	string identifier();
	string intVal();
	string stringVal();
	~JackTokenizer();
	//test
	string getWord();
	string type2Str(TokenType);
};

#endif
