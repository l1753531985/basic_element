#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
	:keywords{new unordered_set<string>}, symbols{new unordered_set<string>}, isFileEnd{false}
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
	return !isFileEnd;
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

//remove note line
string JackTokenizer::getLineFromFile()
{
	string str = "";
	if (ifile.peek() == EOF) isFileEnd = true;
	while (getline(ifile, str))
	{
		if (str.find("/*") != string::npos)
		{
			while (str.find("*/") == string::npos) 
				getline(ifile, str);
			getline(ifile, str);
		}
		str = removeNoteInLine(str, "//");
		if (str.size()) return str;
	}
}

string JackTokenizer::getAToken()
{
	//split line into word
	//cout << "size of vector: " << line2words.size() << endl;
	if (line2words.empty()) 
	{
		line = getLineFromFile();
		if (line.empty()) return "";
		//cout << "line: " << line << endl;
		//cout << "size of line: " << line.size() << endl;
		//sleep(1);
		stringstream aLine{line};
		string word = "";
		while (aLine >> word) line2words.push(word);
	}	

	// split word into token 
	if (words.empty())
	{
		string cur = line2words.front();
		//cout << "cur: " << cur << endl;
		string split = "";
		for (char x : cur)
		{
			if (ispunct(x) && !split.empty())
			{
				words.push(split); 
				string tmp{1,x};
				words.push(tmp);
				split.clear();
				continue;
			}
			split += x;
		}
		words.push(split);
		line2words.pop();
	}
	string ret = words.front();
	cout << "ret: " << ret << endl;
	words.pop();
	return ret; 
}

void JackTokenizer::advance()
{
	getAToken();
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
