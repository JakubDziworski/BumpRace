#include "SingleEliminationWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Macros.h"
#include "Player.h"
#include "SingleElimHUD.h"
#include "Checkpoint.h"
USING_NS_CC;


cocos2d::Scene* SingleEliminationWorld::createScene(int numberOfPlayers, int aiLevel)
{
	auto scene = Scene::create();
	auto gameLayer = SingleEliminationWorld::create(numberOfPlayers,aiLevel);
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = SingleElimHud::create(gameLayer);
	scene->addChild(hudLayer, 2, LAYER_HUD);
	return scene;
}
bool SingleEliminationWorld::myElimInit(int numberOfPlayers, int aiLevel)
{
	if (!World::myInitWithAI(numberOfPlayers,numberOfPlayers-1, aiLevel))
	{
		return false;
	}
	G_dir()->getScheduler()->setTimeScale(1);
	hud = NULL;
	return true;
}

void SingleEliminationWorld::customWorldUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
SingleEliminationWorld* SingleEliminationWorld::create(int numberOfPlayers, int aiLevel)
{
	SingleEliminationWorld *pRet = new SingleEliminationWorld();
	if (pRet && pRet->myElimInit(numberOfPlayers, aiLevel))
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

void SingleEliminationWorld::putOnBoxes()
{
	opponentz.pushBack(Player::create("BOX.png", "KUBA", gravitySpace));
	player = opponentz.at(0);
	for (int i = 1; i < boxesNumber; i++)
	{
		auto aiop = AIOpponent::create("BOX.png", CCString::createWithFormat("AI_%d", i)->getCString(), gravitySpace, aiSmart);
		opponentz.pushBack(aiop);
		aiop->addOrderedOpponents(orderedOpponents);
	}
}

void SingleEliminationWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (hud == NULL) hud = ((SingleElimHud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD));
	if (box == orderedOpponents.at(remainingGates))
	{
		orderedOpponents.at(remainingGates+1)->deactivate();
		hud->displayInfo(CCString::createWithFormat("%s ELIMINATED!", orderedOpponents.at(remainingGates + 1)->getID().c_str())->getCString());
		if (orderedOpponents.at(remainingGates+1) == player) 
			hud->displayGameOver();
	}
	hud->pointsChanged(getSortedBoxesByScore());
}
	

void SingleEliminationWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	Director::getInstance()->replaceScene(SingleEliminationWorld::createScene(boxesNumber, aiSmart));
}

void SingleEliminationWorld::shouldEnableSlowmo(Chcekpoint *chkpt, bool first)
{
	if (first == false)
	{
		chkpt->enableSlowmo();
	}
}

void SingleEliminationWorld::modifyGate(Chcekpoint *inp)
{
	inp->setSprawdzajPierwszych(false);
}

