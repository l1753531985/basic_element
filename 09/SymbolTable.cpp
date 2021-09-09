#include "SymbolTable.h"

SymbolTable::SymbolTable()
	: classScope{new unordered_map<string, Status>}, methodScope{new unordered_map<string, Status>}, data{new unordered_map<string, pair<string, string>>}, staticSegCount{0}, fieldSegCount{0}, argSegCount{0}, varSegCount{0}
{
}

SymbolTable::~SymbolTable()
{
	delete classScope;
	delete methodScope;
	delete data;
}

void SymbolTable::startSubroutine()
{
	methodScope->clear();
}

void SymbolTable::Define(string name, string type, KindType kind)
{
	Status s;
	s.type = type;	
	s.kind = kind;

	switch (kind)
	{
		case KindType::STATIC:
			s.index = staticSegCount;
			classScope->insert({name, s});
			staticSegCount++;
			break;
		case KindType::FIELD:
			s.index = fieldSegCount;
			classScope->insert({name, s});
			fieldSegCount++;
			break;
		case KindType::ARG:
			s.index = argSegCount;
			methodScope->insert({name, s});
			argSegCount++;
			break;
		case KindType::VAR:
			s.index = varSegCount;
			methodScope->insert({name, s});
			varSegCount++;
			break;
		default:
			break;
	}
}

int SymbolTable::varCount(KindType kind)
{
	switch (kind)
	{
		case KindType::STATIC:
		case KindType::FIELD:
			return classScope->size();
		case KindType::ARG:
		case KindType::VAR:
			return methodScope->size();
		default:
			return -1;
	}
}

unordered_map<string, Status>::iterator SymbolTable::getIter(string name)
{
	unordered_map<string, Status>::iterator iter = classScope->find(name);	
	if (iter != classScope->end())
		return iter;

	iter = methodScope->find(name);
	if (iter != methodScope->end())
		return iter;

	return iter;
}

KindType SymbolTable::kindOf(string name)
{
	unordered_map<string, Status>::iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).kind;
	return KindType::NONE;
}

string SymbolTable::TypeOf(string name)
{
	unordered_map<string, Status>::iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).type;
	return "";
}

int SymbolTable::IndexOf(string name)
{
	unordered_map<string, Status>::iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).index;
	return -1;
}	

void SymbolTable::getData(unordered_map<string, pair<string, string>>* resData)
{
	if (resData) data = resData;	
}

//for test
void SymbolTable::printData()
{
	unordered_map<string, pair<string, string>>::iterator iter;
	for (iter = data->begin(); iter != data->end(); iter++)
		cout << "name: " << iter->first << "\tkind: " << iter->second.first << "\ttype: " << iter->second.second << endl;
}
