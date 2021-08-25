#include "CompilationEngine.h"

CompilationEngine::CompilationEngine()
{
}

CompilationEngine::~CompilationEngine()
{
}

void CompilationEngine::CompileClass(istream& ist)
{
	cout << "<class>"<< endl;
	string str;
	while (ist >> str) 
		cout << str << endl;
}
