#ifndef ___BOXX__H_
#define ___BOXX__H_
#include "Boxx.h"

class Player : public Boxx
{
public:
	bool myInit(const std::string& filename, std::string ID, cpSpace *space);
	static Player* create(const std::string& filename, std::string ID, cpSpace *space);
};
#endif
