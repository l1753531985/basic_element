#include "CodeWriter.h"

CodeWriter::CodeWriter(string file_name)
	:default_file_name{file_name}, CODEFLAG1{0}, CODEFLAG2{0}, cmd2Asm{new unordered_map<ArithemticCmd, string>}, str2Amcmd{new unordered_map<string, ArithemticCmd>}, str2Seg{new unordered_map<string, Segment>}, seg2Addr{new unordered_map<Segment, string>}	
{
	asm_file.open(default_file_name+".asm");
	str2Seg->insert({{"constant", CONSTANT}, {"local", LOCAL}, {"argument", ARGUMENT}, {"this", THIS}, {"that", THAT}, {"pointer", POINTER}, {"temp", TEMP}, {"static", STATIC}});
	string static_addr = default_file_name + ".";
	seg2Addr->insert({{Segment::CONSTANT, ""}, {Segment::LOCAL, "LCL"}, {Segment::ARGUMENT, "ARG"}, {Segment::THIS, "THIS"}, {Segment::THAT, "THAT"}, {Segment::TEMP, "5"}, {Segment::STATIC, static_addr}});
	str2Amcmd->insert({{"add", ArithemticCmd::ADD},{"sub", ArithemticCmd::SUB}, {"neg", ArithemticCmd::NEG}, {"eq", ArithemticCmd::EQ}, {"gt", ArithemticCmd::GT}, {"lt", ArithemticCmd::LT}, {"and", ArithemticCmd::AND}, {"or", ArithemticCmd::OR}, {"not", ArithemticCmd::NOT}});

	string common_str_pre = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\n";
	string common_str_post = "\n@SP\nM=M+1\n";
	cmd2Asm->insert({{ArithemticCmd::ADD, common_str_pre+"M=M+1"+common_str_post}, {ArithemticCmd::SUB, common_str_pre+"M=M-D"+common_str_post}, {ArithemticCmd::AND, common_str_pre+"M=D&M"+common_str_post}, {ArithemticCmd::OR, common_str_pre+"M=D|M"+common_str_post}});

	string common_str_pre2 = "@SP\nM=M-1\nA=M\n";
	string common_str_post2 = "\n@SP\nM=M+1\n";
	cmd2Asm->insert({{ArithemticCmd::NEG, common_str_pre2+"M=-M"+common_str_post2}, {ArithemticCmd::NOT, common_str_pre2+"M=!M"+common_str_post2}});

	cmd2Asm->insert({{ArithemticCmd::EQ, "D;JEQ"}, {ArithemticCmd::GT, "D;JGT"}, {ArithemticCmd::LT, "D;JLT"}});
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

string CodeWriter::cmp_asm_str(string core_part)
{
	string append_str = "";
	string CODEFLAG1_str = to_string(CODEFLAG1);
	string CODEFLAG2_str = to_string(CODEFLAG2);
	CODEFLAG1++;
	CODEFLAG2++;

	string str_cmp_cmd_pre = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nD=M-D\n@RET_TRUE"+CODEFLAG1_str+"\n";
	string str_cmp_cmd_post = "\nD=0\n@CONTINUE"+CODEFLAG2_str+"\n0;JMP\n(RET_TRUE"+CODEFLAG1_str+")\nD=-1\n(CONTINUE"+CODEFLAG2_str+")\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	string complete_cmd_str = str_cmp_cmd_pre + core_part + str_cmp_cmd_post; 

	return complete_cmd_str;
}

void CodeWriter::writeArithmetic(string specific_cmd) 
{
	string real_cmd = "";
	for (char x : specific_cmd)
		if (isalpha(x)) 
			real_cmd += x;
	ArithemticCmd cmd = str2Amcmd->find(real_cmd)->second;
	string asm_cmd = cmd2Asm->find(cmd)->second;
	//cout << "asm_cmd: " << asm_cmd << endl;
	if (cmd == ArithemticCmd::EQ || cmd == ArithemticCmd::LT || cmd == ArithemticCmd::GT)		
		asm_cmd = cmp_asm_str(asm_cmd);
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
