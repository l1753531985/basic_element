#include "CodeWrite.h"

CodeWrite::CodeWrite(string ifile_name)
	:file_name{ifile_name}
{
	ifs.open(ifile_name);
	if (!ifs.is_open())
		return -1;
}

CodeWrite::~CodeWrite()
{
	if (ifs.is_open())
		ifs.close();
	if (asm_file.is_open())
		asm_file.close();
	if (hack_file.is_open())
		hack_file.close();
}

void CodeWrite::setFileName(string file_name) 
{
	this->file_name = file_name;
}

void CodeWrite::writeArithmetic(string special_cmd) 
{
}

void CodeWrite::writePushPop(Command cmd)
{
}
