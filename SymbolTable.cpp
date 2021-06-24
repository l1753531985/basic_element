#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable()
	:symbol_map{new unordered_map<string, int>}, variable_to_ram{16}
{
	symbol_map->insert({{"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}});
	symbol_map->insert({{"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}});
	symbol_map->insert({{"SCREEN", 16384}, {"KBD", 24576}});
}

void SymbolTable::addEntry(string symbol, int address)
{
	if (contains(symbol)) return;
	pair<string, int> symbol_pair(symbol, address);
	symbol_map->insert(symbol_pair);
}

void SymbolTable::addEntry(string symbol)
{
	if (contains(symbol)) return;
	addEntry(symbol, variable_to_ram);
	variable_to_ram++;
}

unordered_map<string, int>::iterator SymbolTable::get(string symbol) 
{
	string tmp = "";
	for (char x : symbol)
		if (!isspace(x))
			tmp += x;
	return symbol_map->find(tmp);
}

bool SymbolTable::contains(string symbol)
{
	return (get(symbol) == symbol_map->end()) ? false : true;
}

int SymbolTable::getAddress(string symbol)
{
	return get(symbol)->second;
}

int SymbolTable::getVariableRamAddress()
{
	return variable_to_ram; 
}

SymbolTable::~SymbolTable()
{
	delete symbol_map;
}

//test
void SymbolTable::showTable()
{
	unordered_map<string, int>::iterator iter;
	for (iter = symbol_map->begin(); iter != symbol_map->end(); iter++) 
		cout << iter->first << ": " << iter->second << endl;
}
