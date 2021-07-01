#include <fstream>
#include "Parser.h"

using namespace std;

class CodeWrite {
private:
	string file_name;
	ifstream vm_file;
	fstream asm_file;
	ofstream hack_file;
public:
	CodeWrite(string);
	void setFileName(string);
	void writeArithmetic(string);
	void writePushPop(Command);	
	~CodeWrite();
};
