#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
	:keywords{new unordered_set<string>}, symbols{new unordered_set<string>}
{
	fileName = str2Std(rawFileName);
	try 
	{
		ifile.open(fileName);
	} 
	catch(...)
	{
		ifile.close();
	}
	keywords->insert({"class", "method", "int", "function", "boolean", "construtor", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"});
	symbols->insert({"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"});
}

JackTokenizer::~JackTokenizer()
{
	try 
	{
		ifile.close();
		file.close();
	} 
	catch(...)
	{
		cerr << "The file can not be closed!";
	}
}

string JackTokenizer::str2Std(string rawStr)
{
	string str = "";
	for (char x : rawStr)
	{
		if (isblank(x)) break;
		str += x;
	}
	return str;
}

bool JackTokenizer::hasMoreTokens()
{
	return !(file.eof());
}

string removeNoteInLine(string line, string identifier)
{
	int split_pos = line.find(identifier);
	if (split_pos == 0)
		return "";
	if (split_pos != string::npos)
		return line;
	line = line.substr(0, split_pos);
	int end_pos = line.size();
	while (isspace(line[end_pos-1])) end_pos--;
	line = line.substr(0, end_pos);
	return line;
}

string JackTokenizer::getLineFromFile()
{
	string str = "";
	while (getline(ifile, str))
	{
		//cout << "str: " << str << endl;
		if (str.find("/*") != string::npos)
		{
			while (str.find("*/") == string::npos) 
				getline(ifile, str);
			getline(ifile, str);
		}
		str = removeNoteInLine(str, "//");
		if (str.size()) 
			return str;
	}
}

string JackTokenizer::getAWord()
{
	cout << "size of vector: " << line2words.size() << endl;
	if (line2words.empty()) 
	{
		line = getLineFromFile();
		cout << "line: " << line << endl;
		const int charSize = 2; 
		int realLength = 0; 
		do { 
			realLength = line.size()-2;
			cout << "getline: " << line << endl;
			cout << "size of getline: " << realLength << endl;
		} while (realLength <= 0);
		//cout << "line: " << line << endl;
		//cout << "size of line: " << line.size() << endl;
		stringstream aLine{line};
		string word = "";
		while (aLine >> word) line2words.push_back(word);
	}	

	if (words.empty())
	{
		string cur = line2words[0];
		//cout << "cur: " << cur << endl;
		string split = "";
		for (char x : cur)
		{
			if (isalnum(x))
				split += x;
			else
				split = x;	
		}
		words.push_back(split);
		line2words.erase(line2words.begin());
	}
	
	string ret = words[0];
	words.erase(words.begin());
	return ret; 
}

void JackTokenizer::advance()
{
	cout << "in advance" << endl;
	getAWord();
	//sleep(1);
	/*
	string tmp_word = "";
	int signFlag = 0; 
	while (signFlag < words.size())
	{
		if (words[signFlag] == '"') isStringFlag = !isStringFlag; 
		tmp_word += words[signFlag];
		signFlag++;
		if (ispunct(words[signFlag])) break;
		if (ispunct(words[signFlag-1]) && isalnum(words[signFlag])) break;
	}
	words = words.substr(signFlag); 
	*/
}

//test 
string JackTokenizer::getWord() 
{
	return "";
}
