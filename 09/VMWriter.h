#include <fstream>

class VMWriter {
private:
	enum Segment {CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP};
	enum Command {ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT};
	ofstream ofile;
public:
	VMWriter();
	~VMWriter();
	void Constructor(string);
	void writePush(Segment, int);
	void writePop(Segment, int);
	void writeArithmetic(Command); 
	writeLabel(string);
	writeGoto(string);
	writeIf(string);
	writeCall(string, int);
	writeFunction(string, int);
	writeReturn();
	close();
};
