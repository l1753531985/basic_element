#include "Code.h"

Code::Code()
	:compInBit{new unordered_map<string, bitset<7>>}
{
	unordered_map<string, bitset<7>> tmp = *compInBit;
	compInBit->insert({"0", bitset<7>{"0101010"}});
	compInBit->insert({"1", bitset<7>{"0111111"}});
	compInBit->insert({"-1", bitset<7>{"0111010"}});
	compInBit->insert({"D", bitset<7>{"0001100"}});
	compInBit->insert({"A", bitset<7>{"0110000"}});
	compInBit->insert({"M", bitset<7>{"1110000"}});
	compInBit->insert({"!D", bitset<7>{"0001101"}});
	compInBit->insert({"!A", bitset<7>{"0110001"}});
	compInBit->insert({"!M", bitset<7>{"1110001"}});
	compInBit->insert({"-D", bitset<7>{"0001111"}});
	compInBit->insert({"-A", bitset<7>{"0110011"}});
	compInBit->insert({"-M", bitset<7>{"1110011"}});
	compInBit->insert({"D+1", bitset<7>{"0011111"}});
	compInBit->insert({"A+1", bitset<7>{"0110111"}});
	compInBit->insert({"M+1", bitset<7>{"1110111"}});
	compInBit->insert({"D-1", bitset<7>{"0001110"}});
	compInBit->insert({"A-1", bitset<7>{"0110010"}});
	compInBit->insert({"M-1", bitset<7>{"1110010"}});
	compInBit->insert({"D+A", bitset<7>{"0000010"}});
	compInBit->insert({"D+M", bitset<7>{"1000010"}});
	compInBit->insert({"D-A", bitset<7>{"0010011"}});
	compInBit->insert({"D-M", bitset<7>{"1010011"}});
	compInBit->insert({"A-D", bitset<7>{"0000111"}});
	compInBit->insert({"M-D", bitset<7>{"1000111"}});
	compInBit->insert({"D&A", bitset<7>{"0000000"}});
	compInBit->insert({"D&M", bitset<7>{"1000000"}});
	compInBit->insert({"D|A", bitset<7>{"0010101"}});
	compInBit->insert({"D|M", bitset<7>{"1010101"}});
}

Code::~Code()
{
	delete compInBit;
}

bitset<3> Code::dest2bit(string dest)
{
	string bits = "000";
	int pos_A = dest.find("A");
	int pos_D = dest.find("D");
	int pos_M = dest.find("M");
	if (pos_A != string::npos) bits[0] = '1';
	if (pos_D != string::npos) bits[1] = '1';
	if (pos_M != string::npos) bits[2] = '1';

	return bitset<3>(bits);

}

bitset<3> Code::jump2bit(string jump)
{
	string bits = "000";
	if (jump == "NULL") return bitset<3>(bits);
	if (jump.find('P') != string::npos) 
		bits = "111";
	else if (jump.find("N") != string::npos)
		bits = "101";
	else
	{
		int pos_G = jump.find("G");
		int pos_L = jump.find("L");
		int pos_E = jump.find("E");
		if (pos_G != string::npos) bits[2] = '1';
		if (pos_L != string::npos) bits[0] = '1';
		if (pos_E != string::npos) bits[1] = '1';
	}
	
	return bitset<3>(bits);  
}

bitset<7> Code::comp2bit(string comp)
{
	string tmp = "";
	for (char x : comp) 
		if (!isspace(x))
			tmp += x;

	//cout << "tmp: " << tmp << endl;
      	//cout << "lendth: " << tmp.size() << endl;
	unordered_map<string, bitset<7>>::iterator it = compInBit
->find(tmp);
	return it->second;
}
