#include "AIOpponent.h"
#include "Globals.h"
USING_NS_CC;
void AIOpponent::simulate(float dt)
{
	auto world = G_getWorld();
	if (!world->hasStarted()) return;
	if (world->isPaused() || world->isGameOver()) return;
	if (orderedOpponents == NULL) return;
	if (powerUpExecuted && pwrupType == PowerUp::PowerUpType::GHOST) return;
	if (powerUpExecuted && pwrupType == PowerUp::PowerUpType::SPEED) return;
	if (orderedOpponents->size() == 0) return;
	this->setPrzedniTylni();
	if (tylni && tylni->getVelocityX() > 1.2f*this->getVelocityX() && tylni->isJumping())
	{
		jump();
	}
	else if (przedni && przedni->getVelocityX() < 0.8f*this->getVelocityX())
	{
		jump();
	}
	else if (stykasie())
	{
		jump();
	}
	if (pwrupType != PowerUp::PowerUpType::NONE) maintainPowerUp();
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
void AIOpponent::maintainPowerUp()
{
	Boxx *target=NULL;
	switch (pwrupType)
	{
	case PowerUp::PowerUpType::SPEED:
		if (tylni && abs(tylni->getPositionY() - this->getPositionY()) > this->getContentSize().height
				  && abs(tylni->getPositionX() - this->getPositionX()) < 2*this->getContentSize().width)
		{
			target = tylni;
		}
		else if (przedni && abs(przedni->getPositionY() - this->getPositionY()) > this->getContentSize().height
						 && abs(przedni->getPositionX() - this->getPositionX()) < 2 * this->getContentSize().width)
		{
			target = przedni;
		}
		break;
	case PowerUp::PowerUpType::GHOST:
		if (!przedni) break;
		if (racePos == G_getWorld()->getBoxesNumber()
			&& przedni->getPositionX() - this->getPositionX() < 2*this->getContentSize().width)
		{
			target = przedni;
		}
		break;
	case PowerUp::PowerUpType::THUNDER:
		if (tylni && tylni->getPositionX() - this->getPositionX() < 4 * this->getContentSize().width
				  && abs(tylni->getPositionY() - this->getPositionY()) >= this->getContentSize().height)
		{
			target = tylni;
		}
		break;
	case PowerUp::PowerUpType::NONE:
		break;
	default:
		break;
	}
	if (target)
	{
		if (target == G_getWorld()->getPlayer())
		{
			const int randVal = rand() % 10;
			switch (smartness)
			{
				case 0:
					if (randVal > 5) activatePowerUp();
					break;
				case 1:
					if (randVal > 3) activatePowerUp();
					break;
				case 2:
					activatePowerUp();
					break;
			}
		}
		activatePowerUp();
	}
}
