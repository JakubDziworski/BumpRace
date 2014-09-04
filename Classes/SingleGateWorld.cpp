#include "SingleGateWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Macros.h"
#include "Player.h"
#include "SingleHud.h"
#include "Checkpoint.h"
#include "soundManager.h"
#include "dialogReader.h"
USING_NS_CC;


cocos2d::Scene* SingleGateWorld::createScene(int numberOfPlayers, int gatess, int aiLevel)
{
	auto scene = Scene::create();
	auto gameLayer = SingleGateWorld::create(numberOfPlayers, gatess,aiLevel );
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = SingleGateHud::create();
	scene->addChild(hudLayer, 2, LAYER_HUD);
	gameLayer->setHud(hudLayer);
	return scene;
}
bool SingleGateWorld::myInit(int numberOfPlayers,int gates, int aiLevel)
{
	G_flatTopFilePath = R_flatTopCollector;
	G_drzewkaFilePath = R_drzewkaCollector;
	G_bgFilePath = R_tloCollector;
	if (!World::myInitWithAI(numberOfPlayers,gates, aiLevel))
	{
		return false;
	}
	G_dir()->getScheduler()->setTimeScale(1);
	return true;
}
void SingleGateWorld::customWorldUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
SingleGateWorld* SingleGateWorld::create(int numberOfPlayers,int gatess, int aiLevel)
{
	SingleGateWorld *pRet = new SingleGateWorld();
	if (pRet && pRet->myInit(numberOfPlayers, gatess,aiLevel))
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
void SingleGateWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (pos == 1)
	{
		box->addPoint();
		box->positiveGateAction();
		if (remainingGates != 0)
		{
			SoundManager::getInstance()->playEffect(R_MP3_punch);
            if(remainingGates%10 > 4)
                G_getHud()->displayInfo(String::createWithFormat(G_str("gatesLeft").c_str(), remainingGates)->getCString());
            else if(remainingGates%10 > 1)
                G_getHud()->displayInfo(String::createWithFormat(G_str("gatesLeft2").c_str(), remainingGates)->getCString());
            else if(remainingGates == 1)
                G_getHud()->displayInfo(G_str("lastGate"));
        }
	}
	if (remainingGates == 0)
	{
		if (player) //czy to jest single player
		{
			if (getSortedBoxesByScore()->at(0)->getScore() <= player->getScore())
			{
				this->gameIsOver(true);
			}
			else this->gameIsOver(false);
		}
		else
		{
			this->gameIsOver(true);
		}
	}
	if(dynamic_cast<Player*>(box))
	G_getHud()->pointsChanged(getSortedBoxesByScore());
}
void SingleGateWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	auto scene = SingleGateWorld::createScene(boxesNumber, gatesNumber, aiSmart);
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	this->replaceSceneGenereal(scene, world);
	if (carrerLevel != 0) world->setCarrierLevel(carrerLevel);
}
void SingleGateWorld::shouldEnableSlowmo(Chcekpoint *chkpt, bool first)
{
}
void SingleGateWorld::onDisplayModeTutorial()
{
	DialogReader::getInstance()->getTutorialDialog("gateModeTut.json", G_getHud()->getCocostudioNode(), this);
}

