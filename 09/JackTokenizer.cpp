#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
	:keywords{new unordered_set<string>}, symbols{new unordered_set<string>}, line2words{new queue<string>}, words{new queue<string>}, lines{new queue<string>}, token{""}
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
	getLineFromFile();
	lineSplitIntoWords();
	wordSplitIntoTokens();
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
	delete keywords;
	delete symbols;
	delete lines;
	delete line2words;
	delete words;
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
	return !(words->empty()); 
}

string removeNoteInLine(string line, string identifier)
{
	int split_pos = line.find(identifier);
	if (split_pos == 0)
		return "";
	if (split_pos == string::npos)
		return line;
	line = line.substr(0, split_pos);
	int end_pos = line.size();
	while (isspace(line[end_pos-1])) end_pos--;
	line = line.substr(0, end_pos);
	return line;
}

//remove note line
void JackTokenizer::getLineFromFile()
{
	const int minLineSize = 1;
	string str = "";
	while (getline(ifile, str))
	{
		if (str.find("/*") != string::npos)
		{
			while (str.find("*/") == string::npos) 
				getline(ifile, str);
			getline(ifile, str);
		}
		str = removeNoteInLine(str, "//");
		if (str.size() > minLineSize) lines->push(str);
	}
}

//split line into words
void JackTokenizer::lineSplitIntoWords()
{
	//cout << "size of lines: " << lines->size() << endl;
	while (!lines->empty()) 
	{
		line = lines->front();
		//cout << "line: " << line << endl;
		//cout << "size of line: " << line.size() << endl;
		//sleep(1);
		lines->pop();
		stringstream aLine{line};
		string word = "";
		while (aLine >> word) line2words->push(word);
	}	
}

// split word into tokens 
void JackTokenizer::wordSplitIntoTokens() 
{
	while (!line2words->empty())
	{
		string cur = line2words->front();
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
		//cout << "new_cur: " << new_cur << endl;
		//cout << "after size: " << new_cur.size() << endl;
		stringstream rawTokens{new_cur};
		string real_token = "";
		while (rawTokens >> real_token) words->push(real_token);
		line2words->pop();
	}
}

void JackTokenizer::nextToken()
{
	if (words->empty()) return;
	stringBuffer.push(words->front());
	words->pop();
	if (stringBuffer.front().find('"') != string::npos)
	{  
		do {
			stringBuffer.push(words->front());
			words->pop();
		} while (stringBuffer.back().find('"') == string::npos);
	}
	//cout << "size of buffer: " << stringBuffer.size() << endl;

	if (stringBuffer.size() == 1) 
	{
		token = stringBuffer.front();
		stringBuffer.pop();
	}
	else 
	{
		token.clear();	
		while (!stringBuffer.empty()) 
		{
			token += stringBuffer.front() + " ";
			stringBuffer.pop();
		}
	}
}

void JackTokenizer::advance()
{
	nextToken();
	token2StdChar();
	//cout << "token: "<< token << endl;
	//sleep(1);
}

void JackTokenizer::token2StdChar()
{
	for (unordered_set<string>::iterator iter = symbols->begin(); iter != symbols->end(); iter++)
		if (token.find(*iter) != string::npos)
		{
			token = *iter;
			return;
		}
}

TokenType JackTokenizer::tokenType()
{
	//cout << token << endl;
	if (keywords->find(token) != keywords->end())
		return TokenType::KEYWORD; 
	else if (symbols->find(token) != symbols->end())
		return TokenType::SYMBOL;
	else if (token.find('"') != string::npos)
		return TokenType::STRING_CONST;
	else 
	{
		for (char x : token)
			if (!isdigit(x))
				return TokenType::IDENTIFIER;
		return TokenType::INT_CONST;
	}
}

string JackTokenizer::keyword()
{ return token; }

string JackTokenizer::symbol()
{ 
	if (token == ">") return "&gt;";
	else if (token == "<") return "&lt;";
	else if (token == "&") return "&amp";
	else return token; 
}

string JackTokenizer::identifier()
{ return token; }

int JackTokenizer::intVal()
{ return atoi(token.c_str()); }

string JackTokenizer::stringVal()
{
	int indexStart = token.find('"')+1;
	int length = token.rfind('"') - indexStart;
	return token.substr(indexStart, length);
}

//test 
string JackTokenizer::getWord() 
{
	return token;
}

string JackTokenizer::type2Str(TokenType type)
{
	if (type == TokenType::KEYWORD)
		return "keyword";
	else if (type == TokenType::SYMBOL)
		return "symbol";
	else if (type == TokenType::IDENTIFIER)
		return "identifier";
	else if (type == TokenType::INT_CONST)
		return "int_const";
	else if (type == TokenType::STRING_CONST)
		return "string_const";
	else
		return "not any type";
}
