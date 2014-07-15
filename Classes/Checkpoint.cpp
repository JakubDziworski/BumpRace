//
//  Checkpoint.cpp
//  BumpRaceAndro
//
//  Created by kuba on 11.07.2014.
//
//

#include "Checkpoint.h"
#include "Player.h"
using namespace cocos2d;
Chcekpoint * Chcekpoint::create(World *worldd, cocos2d::Vector<Boxx*> *boxess,std::string imagefileName)
{
	Chcekpoint *pRet = new Chcekpoint();
	if (pRet && pRet->init(worldd, boxess, imagefileName))
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
bool Chcekpoint::init(World *worldd, cocos2d::Vector<Boxx*> *boxess, std::string imagefileName)
{
	if (!Sprite::initWithSpriteFrameName(imagefileName))
	{
		return false;
	}
	this->setAnchorPoint(Vec2(0, 0));
	orderedBoxes = boxess;
	world = worldd;
	actualpos = 0;
	director = Director::getInstance();
	timee = 0;
	pierwszyZlapal = false;
	schedule(schedule_selector(Chcekpoint::tick));
	return true;
}

void Chcekpoint::tick(float dt)
{
	if (actualpos == world->getBoxesNumber())	//WSZYSTKIE PRZESZLY
	{
		if (world->nodeOutOfWindow(this) && director->getScheduler()->getTimeScale() == 1)
		{

			this->removeFromParentAndCleanup(true);
		}
		return;
	}
	//****************//PRZYPISANIE
	Boxx *pierwszy = orderedBoxes->at(0); //pierwszy
	if (pierwszy->getPositionX() < this->getPositionX() - 2.5f * pierwszy->getContentSize().width) return;	//blisko bramki
	Boxx *closestPlayer = orderedBoxes->at(world->getBoxesNumber() - 1); //ostani
	Boxx *aktualny = orderedBoxes->at(actualpos); //aktualny
	//sprawdzenie czy ktos przekroczyl
	if (aktualny->getBoundingBox().getMaxX() > this->getPositionX())
	{
		if (actualpos == 0) pierwszyZlapal = true;
		world->checkpointReached(aktualny,actualpos+1);
		actualpos++;
	}
	//***************// SPRADZENIE CCZY JEST BLISKO PLAYER ABY WLACZYC SLOWMO
	if (actualpos > 0) return;
	bool playerWzasiegu = false;
	if ((dynamic_cast<Player*> (pierwszy)) != NULL)	//pierwszy to player
	{
		for (int i = 1; i < world->getBoxesNumber(); i++)
		{
			Boxx* curr = orderedBoxes->at(i);
			if (pierwszy->getPositionX() < (pierwszy->getContentSize().width + curr->getContentSize().width + curr->getPositionX()))
			{
				playerWzasiegu = true;
				break;
			}
		}
	}
	else //pierwszy to nie player
	{
		for (int i = 1; i < world->getBoxesNumber(); i++)
		{
			Boxx* curr = orderedBoxes->at(i);
			if ((dynamic_cast<Player*> (curr)) != NULL)
			{
				if (pierwszy->getPositionX() < (pierwszy->getContentSize().width + curr->getContentSize().width + curr->getPositionX()))
				{
					playerWzasiegu = true;
					break;
				}
			}
		}
	}
	if (playerWzasiegu == false) return; // zaden gracz nie jest blisko pierwszego
	//*************//
	schedule(schedule_selector(Chcekpoint::zwolnij));
}

void Chcekpoint::przyspiesz(float dt)
{
}

void Chcekpoint::zwolnij(float dt)
{
	if (timee > 0.5f)
	{
		director->getScheduler()->setTimeScale(1);
		schedule(schedule_selector(Chcekpoint::przyspiesz));
		this->unschedule(schedule_selector(Chcekpoint::zwolnij));
	}
	if (director->getScheduler()->getTimeScale() <= 0.1f)
	{
		if (pierwszyZlapal)
		timee += 10*dt;
		return;
	}
	
	director->getScheduler()->setTimeScale(director->getScheduler()->getTimeScale() - 10 * dt);
}

