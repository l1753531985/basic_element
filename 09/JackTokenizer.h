#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <unistd.h>


using namespace std;

enum TokenType {KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST};

class JackTokenizer {
private:
	string fileName;
	ifstream ifile;
	fstream file;
	string line;
	string str2Std(string);
	unordered_set<string>* keywords;
	unordered_set<string>* symbols;
	vector<string> line2words;
	vector<string> words;
	string getAWord();
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
