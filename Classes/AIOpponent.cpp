#include "AIOpponent.h"
USING_NS_CC;
void AIOpponent::simulate(float dt)
{
	if (orderedOpponents->size() == 0) return;
	this->setPrzedniTylni();
	if (tylni && tylni->getVelocity() > 1.2f*this->getVelocity() && tylni->isJumping())
	{
		jump();
	}
	if (przedni && przedni->getVelocity() < 0.8f*this->getVelocity())
	{
		jump();
	}
}

bool AIOpponent::myInit(const std::string& filename,std::string ID, cpSpace *space)
{
	if (!Boxx::myInit(filename, ID,space))
	{
		return false;
	}
	schedule(schedule_selector(AIOpponent::simulate), 0.2f);
	return true;
}

AIOpponent* AIOpponent::create(const std::string& filename, std::string ID, cpSpace *space)
{
	AIOpponent *pRet = new AIOpponent();
	if (pRet && pRet->myInit(filename,ID, space))
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

