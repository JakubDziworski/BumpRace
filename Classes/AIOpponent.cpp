#include "AIOpponent.h"
USING_NS_CC;
void AIOpponent::simulate(float dt)
{
	if (orderedOpponents == NULL) return;
	if (orderedOpponents->size() == 0) return;
	this->setPrzedniTylni();
	if (tylni && tylni->getVelocityX() > 1.1f*this->getVelocityX() && tylni->isJumping())
	{
		jump();
	}
	else if (przedni && przedni->getVelocityX() < 0.9f*this->getVelocityX())
	{
		jump();
	}
	else if (stykasie())
	{
		jump();
	}
}
bool AIOpponent::myInit(const std::string& filename, std::string ID, cpSpace *space, int smartnez, cocos2d::Color3B boxColorr)
{
	if (!Boxx::myInit(filename, ID, space, boxColorr))
	{
		return false;
	}
	this->smartness = smartnez;
	orderedOpponents = NULL;
	//**trudnosc**//
	int randomNumber = rand() % 11 - 1;
	switch (smartness)
	{
	case 0:
		schedule(schedule_selector(AIOpponent::simulate),2);
		break;
	case 1:
		schedule(schedule_selector(AIOpponent::simulate), 1.5f);
		break;
	case 2:
		schedule(schedule_selector(AIOpponent::simulate), 0.2f);
		break;
	default:
		break;
	}
	return true;
}
AIOpponent* AIOpponent::create(const std::string& filename, std::string ID, cpSpace *space, int smartnez, cocos2d::Color3B boxColorr)
{
	AIOpponent *pRet = new AIOpponent();
	if (pRet && pRet->myInit(filename, ID, space, smartnez, boxColorr))
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
float AIOpponent::odleglosc(Boxx *box)
{
	return box->getPositionX() - this->getPositionX();
}
void AIOpponent::setPrzedniTylni()
{
	if (racePos == orderedOpponents->back()->getRacePos()) tylni = NULL;
	else tylni =  orderedOpponents->at(racePos);
	
	if (racePos == orderedOpponents->front()->getRacePos()) przedni = NULL;
	else przedni = orderedOpponents->at(racePos - 2);
}
bool AIOpponent::stykasie()
{
	if (przedni == NULL) return false;
	if (przedni->getBoundingBox().getMinX() < 1.05f*this->getBoundingBox().getMaxX())return true;
	return false;
}

