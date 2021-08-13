#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <queue>
#include <unistd.h>


using namespace std;

enum TokenType {KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	fstream file;
	string line;
	bool isFileEnd;
	string str2Std(string);
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
	queue<string> line2words;
	queue<string> words;
	string getAToken();
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
	string getLineFromFile();
	~JackTokenizer();
	//test
	string getWord();
};
