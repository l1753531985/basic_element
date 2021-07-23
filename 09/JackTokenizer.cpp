#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string rawFileName)
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
	return !(ifile.eof());
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
}

void JackTokenizer::advance()
{
	string str = "";
	getline(ifile, str);
	if (str.find("/*") != string::npos)
	{
		while (str.find("*/") == string::npos) 
			getline(ifile, str);
		getline(ifile, str);
	}
	line = removeNoteInLine(str, "//");
}

//test 
string JackTokenizer::getLine() 
{
	return line;
}
