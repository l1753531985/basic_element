#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>


using namespace std;

enum TokenType {KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	string line;
	string str2Std(string);
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
public:
	JackTokenizer(string);
	bool hasMoreTokens();
	void advance();
	TokenType tokenType();
	string keyword();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();
	~JackTokenizer();
	//test
	string getLine();
};
