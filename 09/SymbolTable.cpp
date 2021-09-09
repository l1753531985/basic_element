#include "SymbolTable.h"

SymbolTable::SymbolTable(const queue<string>& identifiers)
	: classScope{new unordered_map<string, Status>}, methodScope{new unordered_map<string, Status>}, data{new unordered_map<string, pair<string, string>>}, identifiersInOrder{identifiers}, staticSegCount{0}, fieldSegCount{0}, argSegCount{0}, varSegCount{0}
{
	str2KindType.insert({{"static", KindType::STATIC}, {"field", KindType::FIELD}, {"argument", KindType::ARG}, {"var", KindType::VAR}});
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
	while (!identifiersInOrder.empty())
	{
		string name = identifiersInOrder.front();
		unordered_map<string, pair<string, string>>::iterator iter = data->find(name);
		if (iter != data->end())
		{
			if (kindOf(name) == KindType::NONE)
				Define(name, iter->second.second, str2KindType[iter->second.first]);
			cout << "name: " << name << "\tindex: " << IndexOf(name) << endl;
		}
		identifiersInOrder.pop();
	}
}
