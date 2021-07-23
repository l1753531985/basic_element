#pragma once
#include <fstream>
#include <string>
#include <iostream>


using namespace std;

enum TokenType {KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST};

enum Keyword {CLASS, METHOD, INT, FUNCTION, BOOLEAN, CONSTRUTOR, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, K_NULL, THIS};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	string line;
	string str2Std(string);
public:
	JackTokenizer(string);
	bool hasMoreTokens();
	void advance();
	TokenType tokenType();
	Keyword keyword();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();
	~JackTokenizer();
	//test
	string getLine();
};
