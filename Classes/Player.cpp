#include "Player.h"
bool Player::myInit(const std::string& filename, std::string ID, cpSpace *space)
{
	if (!Boxx::myInit(filename, ID, space))
	{
		return false;
	}
	return true;
}

Player* Player::create(const std::string& filename, std::string ID, cpSpace *space)
{
	Player *pRet = new Player();
	if (pRet && pRet->myInit(filename, ID, space))
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

