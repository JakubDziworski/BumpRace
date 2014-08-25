//
//  Checkpoint.cpp
//  BumpRaceAndro
//
//  Created by kuba on 11.07.2014.
//
//
#define COCOS2D_DEBUG 2
#include "Checkpoint.h"
#include "Player.h"
#include "Paths.h"
#include "Globals.h"
#include "soundManager.h"
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
	this->setScaleX(G_wF(100)/this->getContentSize().width);
	this->setScaleY(G_wF(1300) / this->getContentSize().height);
	this->setColor(Color3B(0,174, 239));
	this->setOpacity(153);
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
	//ACTIONS
	return true;
}
void Chcekpoint::tick(float dt)
{
	if (G_getWorld()->isGameOver())
	{
		auto fadeout = FadeOut::create(0.5f);
		auto wait = DelayTime::create(G_getFTimeScale(3));
		this->runAction(Sequence::createWithTwoActions(wait, fadeout));
	}
	if (actualpos == world->getBoxesNumber() || (actualpos == world->getRemainingGates()+1 && !sprawdzajPierwszych))	//WSZYSTKIE PRZESZLY
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
	else sprawdzany = world->getPrzedOstaniActive(); //ostatni aktywny
	if (sprawdzany->getPositionX() < this->getPositionX() - 2.5f * sprawdzany->getContentSize().width) return;
	Boxx *aktualny = orderedBoxes->at(actualpos); //aktualny
	//sprawdzenie czy ktos przekroczyl
	if (aktualny->getBoundingBox().getMaxX() > this->getPositionX())
	{
		if (aktualny == sprawdzany && sprawdzajPierwszych)
		{
			if (!pierwszyZlapal)
			{
				this->runAction(TintTo::create(0.15f, 25, 230, 20));
			}
			if (!isLast)
			SoundManager::getInstance()->disableSlowMo();
			pierwszyZlapal = true;
		}
		else if (actualpos == sprawdzany->getRacePos()-2) 
		{
			this->runAction(TintTo::create(0.15f, 25, 230, 20));
			if (!isLast)
			SoundManager::getInstance()->disableSlowMo();
			pierwszyZlapal = true;
		}
		world->checkpointReachedBase(aktualny,actualpos+1);
		actualpos++;
	}
	if (pierwszyZlapal) return;
	//***************// SPRADZENIE CCZY JEST BLISKO PLAYER ABY WLACZYC SLOWMO
	if (sprawdzajPierwszych) checkIfCloseToFirst(sprawdzany);
	else					 checkIfCloseToLast(sprawdzany);
}
void Chcekpoint::checkIfCloseToLast(Boxx *ostatni)
{
	if (slowmoTriggered) return;
	if ((this->getPositionX() - ostatni->getPositionX()) / ostatni->getVelocityX()>3) return;
	if (isLast){ enableSlowmo(); return; }
	bool playerWzasiegu = false;
	if ((dynamic_cast<Player*> (ostatni)) != NULL)	//ostatni to player
	{
		for (int i = ostatni->getRacePos()-2; i >= 0;i--)
		{
			Boxx *curr = orderedBoxes->at(i);
			if (curr->getPositionX() - ostatni->getPositionX() < curr->getContentSize().width + ostatni->getContentSize().width)
			{
				playerWzasiegu = true;
				break;
			}
		}
	}
	else //ostatni to nie player
	{
		for (int i = ostatni->getRacePos()-2; i >= 0; i--)
		{
			Boxx *curr = orderedBoxes->at(i);
			if ((dynamic_cast<Player*> (curr)) != NULL)
			{
				if (curr->getPositionX() - ostatni->getPositionX() < curr->getContentSize().width + ostatni->getContentSize().width)
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
void Chcekpoint::checkIfCloseToFirst(Boxx* pierwszy)
{
	if (slowmoTriggered) return;
	if ((this->getPositionX() - pierwszy->getPositionX()) / pierwszy->getVelocityX() > 3) return;
	if (isLast){ enableSlowmo(); return; }
	bool playerWzasiegu = false;
	if ((dynamic_cast<Player*> (pierwszy)) != NULL)	//pierwszy to player
	{
		for (int i = 1; i < world->getBoxesNumber(); i++)
		{
			Boxx *curr = orderedBoxes->at(i);
			if (pierwszy->getPositionX() - curr->getPositionX() < pierwszy->getContentSize().width + curr->getContentSize().width)
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
			Boxx *curr = orderedBoxes->at(i);
			if ((dynamic_cast<Player*> (curr)) != NULL)
			{
				if (pierwszy->getPositionX() - curr->getPositionX() < pierwszy->getContentSize().width + curr->getContentSize().width)
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
	SoundManager::getInstance()->enableSlowMo();
	slowmoTriggered = true;
	schedule(schedule_selector(Chcekpoint::zwolnij));
}
void Chcekpoint::zwolnij(float dt)
{
	if (timee > 0.5f)
	{
		if (!G_getWorld()->isGameOver())
		{
			director->getScheduler()->setTimeScale(1);
			this->unschedule(schedule_selector(Chcekpoint::zwolnij));
		}
		return;
	}
	if (director->getScheduler()->getTimeScale() <= 0.1f)
	{
		if (pierwszyZlapal)
			timee += 10 * dt;
		return;
	}
}
void Chcekpoint::triggerFirstVisualEffects(Boxx *box)
{
	////auto particleSystem = G_getParticleFromFile(R_checkpointParticle);
	//particleSystem->setStartSize(G_wF(15));
	//particleSystem->setEndSize(G_wF(15));
	//particleSystem->setSpeed(G_wF(500));
	//particleSystem->setSpeedVar(G_wF(500));
	//particleSystem->setPosVar(Vec2(G_wF(35), G_hF(35)));
	//particleSystem->setPosition(Vec2(box->getBoundingBox().getMaxX(),box->getPositionY()));
	////particleSystem->setPosVar(Vec2(G_wF(25), box->getContentSize().height));
	//this->getParent()->addChild(particleSystem);
}
