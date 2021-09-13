#include "VMWriter.h"

VMWriter::VMWriter()
	:classScope{new unordered_map<string, Status>}, methodScope{new unordered_map<string, Status>}
{
}

VMWriter::~VMWriter()
{
	delete classScope;
	delete methodScope;
}

void VMWriter::Constructor(string fileName)
{
	ofile.open(fileName);
}

void VMWriter::close()
{
	if (ofile.is_open())
		ofile.close();
}

unordered_map<string, Status>* VMWriter::getClassScopePtr()
{
	return classScope;
}

unordered_map<string, Status>* VMWriter::getMethodScopePtr()
{
	return methodScope;
}

// for test
void VMWriter::printScope(ostream& os)
{
	unordered_map<string, Status>::iterator iter;
	for (iter = classScope->begin(); iter != classScope->end(); iter++)
		os << "name: " << iter->first << "\tindex: " << iter->second.index << endl;
	for (iter = methodScope->begin(); iter != methodScope->end(); iter++)
		os << "name: " << iter->first << "\tindex: " << iter->second.index << endl;
}
