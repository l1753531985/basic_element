#ifndef __STATUS__
#define __STATUS__

#include <string>

using namespace std;

enum KindType {STATIC, FIELD, ARG, VAR, NONE};

struct Status {
	string type;
	KindType kind; 
	int index;
	//for test
	static string KindType2Str(KindType kind)
	{
		switch (kind)
		{
			case KindType::STATIC:
				return "static";
			case KindType::FIELD:
				return "field";
			case KindType::ARG:
				return "arg";
			case KindType::VAR:
				return "var";
			default:
				return "none"; 	
		}
	}
};
#endif
