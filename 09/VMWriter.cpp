#include "VMWriter.h"

VMWriter::VMWriter()
{
}

VMWriter::~VMWriter()
{
}

void VMWriter::Constructor(string fileName)
{
	ofile.open();
}

void VMWriter::close()
{
	if (ofile.is_open())
		ofile.close();
}
