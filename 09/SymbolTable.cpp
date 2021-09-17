#include "SymbolTable.h"

SymbolTable::SymbolTable()
	: classScope{new unordered_map<string, Status>}, methodScope{new unordered_map<string, Status>}, staticSegCount{0}, fieldSegCount{0}, argSegCount{0}, varSegCount{0}, str2KindType{{"static", KindType::STATIC}, {"field", KindType::FIELD}, {"argument", KindType::ARG}, {"var", KindType::VAR}}
{
}

SymbolTable::~SymbolTable()
{
	delete classScope;
	delete methodScope;
}

void SymbolTable::startSubroutine() const
{
	methodScope->clear();
}

void SymbolTable::Define(string name, string type, KindType kind)
{
	if (kindOf(name) != KindType::NONE) return;
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

int SymbolTable::varCount(KindType kind) const
{
	switch (kind)
	{
		case KindType::STATIC:
			return staticSegCount;
		case KindType::FIELD:
			return fieldSegCount;
		case KindType::ARG:
			return argSegCount;
		case KindType::VAR:
			return varSegCount;
		default:
			return -1;
	}
}

unordered_map<string, Status>::const_iterator SymbolTable::getIter(string name) const
{
	unordered_map<string, Status>::iterator iter = classScope->find(name);	
	if (iter != classScope->end())
		return iter;

	iter = methodScope->find(name);
	if (iter != methodScope->end())
		return iter;

	return iter;
}

KindType SymbolTable::kindOf(string name) const
{
	unordered_map<string, Status>::const_iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).kind;
	return KindType::NONE;
}

string SymbolTable::TypeOf(string name) const
{
	unordered_map<string, Status>::const_iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).type;
	return "";
}

int SymbolTable::IndexOf(string name) const
{
	unordered_map<string, Status>::const_iterator iter = getIter(name);
	if (iter != methodScope->end())
		return (iter->second).index;
	return -1;
}	


KindType SymbolTable::str2Kind(string kind) const 
{
	return str2KindType.at(kind);
}

//for test
void SymbolTable::printAllElem(ostream& os) const
{
	cout << "count of classScope: " << classScope->size() << endl;
	cout << "count of methodScope: " << methodScope->size() << endl;
	unordered_map<string, Status>::iterator iter;
	for (iter = classScope->begin(); iter != classScope->end(); iter++)
		os << iter->first << ": " << iter->second.index << endl;
	for (iter = methodScope->begin(); iter != methodScope->end(); iter++)
		os << iter->first << ": " << iter->second.index << endl;
}
