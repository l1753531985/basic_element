#pragma once
#include <string>
#include <bitset>
#include <iostream>
#include <unordered_map>

using namespace std;



class Code {
private:
	unordered_map<string,bitset<7>>* compInBit;
public:
	Code();
	bitset<3> dest2bit(string);
	bitset<7> comp2bit(string);
	bitset<3> jump2bit(string);
	~Code();
};
