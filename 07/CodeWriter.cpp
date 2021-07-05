#include "CodeWriter.h"

CodeWriter::CodeWriter(string file_name)
	:default_file_name{file_name}, cmd2Asm{new unordered_map<ArithemticCmd, string>}, str2Amcmd{new unordered_map<string, ArithemticCmd>}, str2Seg{new unordered_map<string, Segment>}
{
	asm_file.open(default_file_name+".asm");
	str2Seg->insert({{"constant", CONSTANT}, {"local", LOCAL}, {"argument", ARGUMENT}, {"this", THIS}, {"that", THAT}, {"pointer", POINTER}});
	str2Amcmd->insert({{"add", ArithemticCmd::ADD},{"sub", ArithemticCmd::SUB}, {"neg", ArithemticCmd::NEG}, {"eq", ArithemticCmd::EQ}, {"gt", ArithemticCmd::GT}, {"lt", ArithemticCmd::LT}, {"and", ArithemticCmd::AND}, {"or", ArithemticCmd::OR}, {"not", ArithemticCmd::NOT}});
	cmd2Asm->insert({{ArithemticCmd::ADD, "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=D+M\n@SP\nM=M+1\n"}, {ArithemticCmd::SUB, "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M-D\n@SP\nM=M+1\n"}});
}

CodeWriter::~CodeWriter()
{
	if (asm_file.is_open())
		asm_file.close();
	delete cmd2Asm;
	delete str2Amcmd;
	delete str2Seg;
}

void CodeWriter::setFileName(string file_name) 
{
	if (asm_file.is_open())
		asm_file.close();
	default_file_name = file_name;
	asm_file.open(default_file_name+".asm");
}

void CodeWriter::writeArithmetic(string specific_cmd) 
{
	string real_cmd = "";
	for (char x : specific_cmd)
		if (isalpha(x)) 
			real_cmd += x;
	ArithemticCmd cmd = str2Amcmd->find(real_cmd)->second;
	string asm_cmd = cmd2Asm->find(cmd)->second;
}

void CodeWriter::writePushPop(Command cmd, string seg, int index)
{
	if (cmd != Command::C_POP && cmd != Command::C_PUSH)
		throw "cmd is not pop or push";
	string real_seg = "";
	for (char x : seg)
		if (isalpha(x)) 
			real_seg += x;
	if (cmd == Command::C_PUSH)
	{
		Segment segment = str2Seg->find(real_seg)->second;
		string asm_cmd = "";
		switch (segment)
		{
			case Segment::CONSTANT:
				asm_cmd = "@"+to_string(index)+"\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
				break;
			case Segment::LOCAL:
				asm_cmd = "@LCL\nD=M\n@"+to_string(index)+"\nA=A+D\nD=M\n@SP\nA=M\nM=D@SP\nM=M+1\n";
				break;
			case Segment::ARGUMENT:
				asm_cmd = "@ARG\nD=M\n@"+to_string(index)+"\nA=A+D\nD=M\n@SP\nA=M\nM=D@SP\nM=M+1\n";
				break;
			case Segment::THIS:
				asm_cmd = "@THIS\nD=M\n@"+to_string(index)+"\nA=A+D\nD=M\n@SP\nA=M\nM=D@SP\nM=M+1\n";
				break;
			case Segment::THAT:
				asm_cmd = "@THAT\nD=M\n@"+to_string(index)+"\nA=A+D\nD=M\n@SP\nA=M\nM=D@SP\nM=M+1\n";
				break;
			case Segment::POINTER:
				break;
		}
	}			
	else
	{
		Segment segment = str2Seg->find(real_seg)->second;
		string asm_cmd = "";
		switch (segment)
		{
			case Segment::LOCAL:
				asm_cmd = "@SP\nM=M-1\nA=M\nD=M\n@LCL\nA=M\nA=A+"+to_string(index-1)+"\nM=D\n";
				break;
			case Segment::ARGUMENT:
				asm_cmd = "@SP\nM=M-1\nA=M\nD=M\n@ARG\nA=M\nA=A+"+to_string(index-1)+"\nM=D\n";
				break;
			case Segment::THIS:
				asm_cmd = "@SP\nM=M-1\nA=M\nD=M\n@THIS\nA=M\nA=A+"+to_string(index-1)+"\nM=D\n";
				break;
			case Segment::THAT:
				asm_cmd = "@SP\nM=M-1\nA=M\nD=M\n@THAT\nA=M\nA=A+"+to_string(index-1)+"\nM=D\n";
				break;
			case Segment::POINTER:
				break;
		}
	}
}
