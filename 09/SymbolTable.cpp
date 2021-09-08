#include "SymbolTable.h"

SymbolTable::SymbolTable()
	: classScope{new unordered_map<string, Status>}, methodScope{new unordered_map<string, Status>}, staticSegCount{0}, fieldSegCount{0}, argSegCount{0}, varSegCount{0}
{
}

SymbolTable::~SymbolTable()
{
	delete classScope;
	delete methodScope;
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

KindType SymbolTable::kindOf(string name)
{
	unordered_map<string, Status>::iterator iter = classScope->find(name);	

	if (iter != classScope->end())
		return (iter->second).kind;

	iter = methodScope->find(name);
	if (iter != methodScope->end())
		return (iter->second).kind;

	return KindType::NONE;
}

