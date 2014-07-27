#include "SingleGateWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Macros.h"
#include "Player.h"
#include "SingleHud.h"
#include "Checkpoint.h"
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
	((SingleGateHud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))->pointsChanged(getSortedBoxesByScore());
}
void SingleGateWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	auto scene = SingleGateWorld::createScene(boxesNumber, gatesNumber, aiSmart);
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	this->replaceSceneGenereal(scene, world);
}
void SingleGateWorld::shouldEnableSlowmo(Chcekpoint *chkpt, bool first)
{
}

