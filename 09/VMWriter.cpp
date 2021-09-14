#include "VMWriter.h"

VMWriter::VMWriter(string fileName)
	:fileName{fileName}
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

