#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
	:keywords{new unordered_set<string>}, symbols{new unordered_set<string>}, isFileEnd{false}, statusStrGet{Status::NOTSTRING}
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

//split line into words
void JackTokenizer::lineSplitIntoWords()
{
	//cout << "size of vector: " << line2words.size() << endl;
	if (line2words.empty()) 
	{
		line = getLineFromFile();
		//cout << "line: " << line << endl;
		//cout << "size of line: " << line.size() << endl;
		//sleep(1);
		stringstream aLine{line};
		string word = "";
		while (aLine >> word) line2words.push(word);
	}	
}

// split word into tokens 
void JackTokenizer::wordSplitIntoTokens() 
{
	if (words.empty())
	{
		string cur = line2words.front();
		//cout << "cur: " << cur << endl;
		string new_cur = "";
		for (char x : cur)
		{
			if (ispunct(x)) 
			{
				string tmp{1, x};
				new_cur += " " + tmp + " ";
			}
			else
				new_cur += x;
		}
		stringstream rawTokens{new_cur};
		string real_token = "";
		while (rawTokens >> real_token)
			words.push(real_token);
		line2words.pop();
	}
}

void JackTokenizer::nextToken()
{
	if (statusStrGet == Status::STRSTART) 
	{
		while (words.front().find('"') == string::npos)	
		{
			token += words.front();
			words.pop();
		}
	}
	else
	{
		token = words.front();
		words.pop();
	}

	if (token.find('"') != string::npos)
	{
		if (statusStrGet == Status::NOTSTRING)
			statusStrGet = Status::STRSTART;
		else if (statusStrGet == Status::STRSTART)
			statusStrGet = Status::STREND;
	}
	else if (statusStrGet == Status::STREND)
		statusStrGet = Status::NOTSTRING;
}

void JackTokenizer::advance()
{
	lineSplitIntoWords();
	if (!hasMoreTokens()) return;
	wordSplitIntoTokens();
	nextToken();
	cout << "token: " << token << endl;
	//sleep(1);
}

//test 
string JackTokenizer::getWord() 
{
	return token;
}
