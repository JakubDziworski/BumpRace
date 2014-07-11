//
//  Checkpoint.cpp
//  BumpRaceAndro
//
//  Created by kuba on 11.07.2014.
//
//

#include "Checkpoint.h"
using namespace cocos2d;
Chcekpoint * Chcekpoint::create(World *worldd, cocos2d::Vector<Boxx*> *boxess, std::string imagefileName)
{
	Chcekpoint *pRet = new Chcekpoint();
	if (pRet && pRet->init(worldd, boxess,imagefileName))
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
}

void Chcekpoint::tick(float dt)
{
	if (actualpos == world->getBoxesNumber())
	{
		if (director->getVisibleOrigin().x - director->getVisibleSize().width / 2.0f && director->getScheduler()->getTimeScale() == 1)
		{
			this->removeFromParentAndCleanup(true);
		}
		return;
	}
	Boxx *pierwszy = orderedBoxes->at(0);
	Boxx *drugi = orderedBoxes->at(1);
	if (pierwszy->getPositionX() < this->getPositionX() - 3*pierwszy->getContentSize().width) return;
	if (actualpos == 0 && (pierwszy->getPositionX() < (pierwszy->getContentSize().width + drugi->getContentSize().width) + drugi->getPositionX()))
	{
		schedule(schedule_selector(Chcekpoint::zwolnij));
	}
	Boxx *aktualny = orderedBoxes->at(actualpos);
	if (aktualny->getBoundingBox().getMaxX() > this->getPositionX())
	{
		if (actualpos == 0) pierwszyZlapal = true;
		world->checkpointReached(aktualny,actualpos+1);
		actualpos++;
	}
}

void Chcekpoint::przyspiesz(float dt)
{
}

void Chcekpoint::zwolnij(float dt)
{
	if (timee > 0.7f)
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

