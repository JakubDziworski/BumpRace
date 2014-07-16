#include "AIOpponent.h"
USING_NS_CC;
void AIOpponent::simulate(float dt)
{
	if (orderedOpponents->size() == 0) return;
	this->setPrzedniTylni();
	bool doJump = false;
	if (tylni && tylni->getVelocity() > 1.2f*this->getVelocity() && tylni->isJumping())
	{
		doJump = true;
	}
	if (przedni && przedni->getVelocity() < 0.8f*this->getVelocity())
	{
		doJump = true;
	}
	if (stykasie())
	{
		doJump = true;
	}
	//smartness odrzut
	if (!doJump) return;
	int randomNumber = rand() % 11 -1;
	switch (smartness)
	{
	case 0:
		if (randomNumber <= 3) jump();
		break;
	case 1:
		if (randomNumber <= 6) jump();
		break;
	case 2:
		jump();
		break;
	default:
		break;
	}
}

bool AIOpponent::myInit(const std::string& filename, std::string ID, cpSpace *space, int smartnez)
{
	if (!Boxx::myInit(filename, ID,space))
	{
		return false;
	}
	this->smartness = smartnez;
	schedule(schedule_selector(AIOpponent::simulate), 0.2f);
	return true;
}

AIOpponent* AIOpponent::create(const std::string& filename, std::string ID, cpSpace *space, int smartnez)
{
	AIOpponent *pRet = new AIOpponent();
	if (pRet && pRet->myInit(filename,ID, space,smartnez))
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
	if (przedni->getBoundingBox().getMinX() - this->getBoundingBox().getMaxX() < 2)return true;
	return false;
}

