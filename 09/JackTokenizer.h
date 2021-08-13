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

enum Status {STRSTART, STREND, NOTSTRING};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	string line;
	bool isFileEnd;
	string token;
	Status statusStrGet;
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
	queue<string> line2words;
	queue<string> words;
	string str2Std(string);
	void lineSplitIntoWords();
	void wordSplitIntoTokens();
	void nextToken();
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
