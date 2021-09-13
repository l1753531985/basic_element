#ifndef __STATUS__
#define __STATUS__

#include <string>

using namespace std;

enum KindType {STATIC, FIELD, ARG, VAR, NONE};

struct Status {
	string type;
	KindType kind; 
	int index;
};
#endif
