#include "CodeWriter.h"

CodeWriter::CodeWriter(string file_name)
	:default_file_name{file_name}, cmd2Asm{new unordered_map<ArithemticCmd, string>}, str2Amcmd{new unordered_map<string, ArithemticCmd>}, str2Seg{new unordered_map<string, Segment>}, seg2Addr{new unordered_map<Segment, string>}	
{
	asm_file.open(default_file_name+".asm");
	str2Seg->insert({{"constant", CONSTANT}, {"local", LOCAL}, {"argument", ARGUMENT}, {"this", THIS}, {"that", THAT}, {"pointer", POINTER}, {"temp", TEMP}, {"static", STATIC}});
	string static_addr = default_file_name + ".";
	seg2Addr->insert({{Segment::CONSTANT, ""}, {Segment::LOCAL, "LCL"}, {Segment::ARGUMENT, "ARG"}, {Segment::THIS, "THIS"}, {Segment::THAT, "THAT"}, {Segment::TEMP, "5"}, {Segment::STATIC, static_addr}});
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
	asm_file << asm_cmd;
}

string CodeWriter::push_asm_str(string seg_basic_addr, int index)
{
	string asm_cmd = "";
	if (seg_basic_addr.empty())
		asm_cmd = "@"+to_string(index)+"\n";	
	else if (seg_basic_addr == seg2Addr->find(Segment::STATIC)->second)
		asm_cmd = "@"+seg_basic_addr+to_string(index)+"\n";
	else
		asm_cmd = "@"+seg_basic_addr+"\nD=M\n@"+to_string(index)+"\nA=A+D\n";
	asm_cmd += "D=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	return asm_cmd;
}

string CodeWriter::pop_asm_str(string seg_basic_addr, int index)
{
	string asm_cmd = "@SP\nM=M-1\nA=M\nD=M\n@"+seg_basic_addr;
	if (seg_basic_addr == seg2Addr->find(Segment::STATIC)->second)
		asm_cmd += to_string(index);
	asm_cmd += "\n";
	for (int i = 0; i < index; i++) asm_cmd += "A=A+1\n";
	asm_cmd += "M=D\n";
	return asm_cmd;
}

void CodeWriter::writePushPop(Command cmd, string seg, int index)
{
	if (cmd != Command::C_POP && cmd != Command::C_PUSH)
		throw "cmd is not pop or push";
	string real_seg = "";
	for (char x : seg)
		if (isalpha(x)) 
			real_seg += x;
	string asm_cmd = "";
	Segment segment = str2Seg->find(real_seg)->second;
	string basic_addr = "";
	if (segment == Segment::POINTER)
		basic_addr = (index) ? 4 : 3;
	else
		basic_addr = seg2Addr->find(segment)->second;
	if (cmd == Command::C_PUSH)
		asm_cmd = push_asm_str(basic_addr, index);
	else
		asm_cmd = pop_asm_str(basic_addr, index);
	//cout << asm_cmd << endl;
	asm_file << asm_cmd;
}
