#ifndef ___BOXX__H_
#define ___BOXX__H_
#include "Boxx.h"

class Player : public Boxx
{
public:
	bool myInit(const std::string& filename, std::string ID, cpSpace *space,cocos2d::Color3B boxColorr);
	static Player* create(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr = cocos2d::Color3B::BLACK);
};
#endif
