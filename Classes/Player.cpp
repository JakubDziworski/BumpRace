#include "Player.h"
bool Player::myInit(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr)
{
	if (!Boxx::myInit(filename, ID, space, boxColorr))
	{
		return false;
	}
	return true;
}

Player* Player::create(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr)
{
	Player *pRet = new Player();
	if (pRet && pRet->myInit(filename, ID, space, boxColorr))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

