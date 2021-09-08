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
			pair<string, Status> elem{name, s};
			classScope->insert(elem);
			staticSegCount++;
			break;
		case KindType::FIELD:
			s.index = fieldSegCount;
			pair<string, Status> elem{name, s};
			classScope->insert(elem);
			fieldSegCount++;
			break;
		case KindType::ARG:
			s.index = argSegCount;
			pair<string, Status> elem{name, s};
			methodScope->insert(elem);
			argSegCount++;
			break;
		case KindType::VAR:
			s.index = varSegCount;
			pair<string, Status> elem{name, s};
			methodScope->insert(elem);
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

KindType SymbolTable::keindOf(string name)
{
		
}

