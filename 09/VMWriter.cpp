#include "VMWriter.h"

VMWriter::VMWriter(string fileName)
	:fileName{fileName}, seg2Str{{S_CONST, "constant"}, {S_ARG, "argument"}, {S_LOCAL, "local"}, {S_STATIC, "static"}, {S_THIS, "this"}, {S_THAT, "that"}, {S_POINTER, "pointer"}, {S_TEMP, "temp"}}
{
}

VMWriter::~VMWriter()
{
}

void VMWriter::Constructor()
{
	ofile.open(fileName);
}

void VMWriter::close()
{
	if (ofile.is_open())
		ofile.close();
}

void VMWriter::writeFunction(string funcName, int nargs)
{
	ofile << "function " << funcName << " " << nargs << endl;
}

void VMWriter::writeReturn()
{
	ofile << "return" << endl;
}

void VMWriter::writePush(Segment seg, int index)
{
	ofile << "push " << seg2Str[seg] << " " << index << endl;
}

void VMWriter::writeCall(string name, int nargs)
{
	ofile << "call " << name << " " << nargs << endl;
}

void VMWriter::writePop(Segment seg, int index)
{
	ofile << "pop " << seg2Str[seg] << " " << index << endl;
}
