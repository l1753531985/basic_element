#include "VMWriter.h"

VMWriter::VMWriter()
{
}

VMWriter::~VMWriter()
{
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

// for test
void VMWriter::printScope(ostream& os)
{
	/*
	unordered_map<string, Status>::iterator iter;
	for (iter = classScope->begin(); iter != classScope->end(); iter++)
		os << "name: " << iter->first << "\tkind: " << Status::KindType2Str(iter->second.kind) << "\tindex: " << iter->second.index << endl;
	for (iter = methodScope->begin(); iter != methodScope->end(); iter++)
		os << "name: " << iter->first << "\tkind: " << Status::KindType2Str(iter->second.kind) << "\tindex: " << iter->second.index << endl;
	*/
}
