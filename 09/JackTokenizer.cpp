#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
	:keywords{new unordered_set<string>}, symbols{new unordered_set<string>}
{
	fileName = str2Std(rawFileName);
	string tmpFileName = "./tmpFile.jack";
	try 
	{
		ifile.open(fileName);
		file.open(tmpFileName, ios::in|ios::out);
	} 
	catch(...)
	{
		ifile.close();
		file.close();
	}
	writeCodeToTmpFile();
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
	return (file.peek() == EOF) ? false : true;
}

string removeNoteInLine(string line, string identifier)
{
	int split_pos = line.find(identifier);
	if (split_pos == 0)
		return "";
	if (split_pos < 0)
		return line;
	line = line.substr(0, split_pos);
	int end_pos = line.size();
	while (isspace(line[end_pos-1])) end_pos--;
	line = line.substr(0, end_pos);
	return line;
}

void JackTokenizer::writeCodeToTmpFile()
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
		file << str;
	}
	file.clear();
	file.seekp(0, ios::beg);
}

void JackTokenizer::advance()
{
	if (words.empty()) 
		file >> words;
	string tmp_word = "";
	int signFlag = 0; 
	while (signFlag < words.size())
	{
		tmp_word += words[signFlag];
		signFlag++;
		if (ispunct(words[signFlag])) break;
		if (ispunct(words[signFlag-1]) && isalnum(words[signFlag])) break;
	}
	word = tmp_word;
	words = words.substr(signFlag); 
}

//test 
string JackTokenizer::getWord() 
{
	return word;
}
