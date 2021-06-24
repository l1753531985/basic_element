#pragma once
#include <unordered_map>
#include <string>

using namespace std;

class SymbolTable {
private:
	int variable_to_ram;
	unordered_map<string, int>* symbol_map;
	unordered_map<string, int>::iterator get(string);
public:
	SymbolTable();
	void addEntry(string, int);
	void addEntry(string);
	bool contains(string);
	int getAddress(string);
	~SymbolTable();
	//test
	void showTable();
};
