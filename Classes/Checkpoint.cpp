﻿//
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
	isLast = false;
	pierwszyZlapal = false;
	slowmoTriggered = false;
	sprawdzajPierwszych = true;
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
	Boxx *sprawdzany = NULL;
	Boxx *pierwszy = orderedBoxes->at(0); //pierwszy
	if (sprawdzajPierwszych) sprawdzany = pierwszy; //pierwszy
	else sprawdzany = world->getPrzedostaniActive(); //przedostatni
	if (pierwszy->getPositionX() < this->getPositionX() - 2.5f * pierwszy->getContentSize().width) return;
	Boxx *aktualny = orderedBoxes->at(actualpos); //aktualny
	//sprawdzenie czy ktos przekroczyl
	if (aktualny->getBoundingBox().getMaxX() > this->getPositionX())
	{
		if (actualpos == 0 && sprawdzajPierwszych) pierwszyZlapal = true;
		else if (aktualny == sprawdzany) pierwszyZlapal = true;
		world->checkpointReachedBase(aktualny,actualpos+1);
		actualpos++;
	}
	//***************// SPRADZENIE CCZY JEST BLISKO PLAYER ABY WLACZYC SLOWMO
	if (sprawdzajPierwszych) checkIfClose(sprawdzany,true);
	else					 checkIfClose(sprawdzany,false);
}


void Chcekpoint::checkIfClose(Boxx* sprawdzany,bool first)
{
	if (slowmoTriggered) return;
	if (!first)
	{
		if (sprawdzany->getPositionX() < this->getPositionX() - 2.5f * sprawdzany->getContentSize().width) return;
	}
	bool playerWzasiegu = false;
	if ((dynamic_cast<Player*> (sprawdzany)) != NULL)	//sprawdzany to player
	{
		for (int i = 1; i < world->getBoxesNumber(); i++)
		{
			Boxx* curr = orderedBoxes->at(i);
			if (sprawdzany->getPositionX() < (sprawdzany->getContentSize().width + curr->getContentSize().width + curr->getPositionX()))
			{
				playerWzasiegu = true;
				break;
			}
		}
	}
	else //sprawdzany to nie player
	{
		for (Boxx* curr : *world->getBoxes())
		{
			if ((dynamic_cast<Player*> (curr)) != NULL && curr != sprawdzany)
			{
				if (sprawdzany->getPositionX() < (sprawdzany->getContentSize().width + curr->getContentSize().width + curr->getPositionX()))
				{
					playerWzasiegu = true;
					break;
				}
			}
		}
	}
	if (playerWzasiegu == false) return; // zaden gracz nie jest blisko pierwszego
	//*************//
	enableSlowmo();
}


void Chcekpoint::setSprawdzajPierwszych(bool inp)
{
	sprawdzajPierwszych = inp;
}

void Chcekpoint::enableSlowmo()
{
	director->getScheduler()->setTimeScale(0.1f);
	slowmoTriggered = true;
	schedule(schedule_selector(Chcekpoint::zwolnij));
}

void Chcekpoint::zwolnij(float dt)
{
	if (timee > 0.5f)
	{
		director->getScheduler()->setTimeScale(1);
		this->unschedule(schedule_selector(Chcekpoint::zwolnij));
		return;
	}
	if (director->getScheduler()->getTimeScale() <= 0.1f)
	{
		if (pierwszyZlapal)
			timee += 10 * dt;
		return;
	}
}