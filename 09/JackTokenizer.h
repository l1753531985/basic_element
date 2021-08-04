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
	fstream file;
	string words;
	string word;
	string advance_word;
	bool isStringFlag;
	string str2Std(string);
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
	string tokenStdAdvance();
	string tokenStringAdvance();
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
	void writeCodeToTmpFile();
	~JackTokenizer();
	//test
	string getWord();
};
