#include "SingleEliminationWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Macros.h"
#include "Player.h"
#include "SingleElimHUD.h"
#include "Checkpoint.h"
#include "soundManager.h"
USING_NS_CC;
cocos2d::Scene* SingleEliminationWorld::createScene(int numberOfPlayers, int aiLevel)
{
	auto scene = Scene::create();
	auto gameLayer = SingleEliminationWorld::create(numberOfPlayers,aiLevel);
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = SingleElimHud::create();
	scene->addChild(hudLayer, 2, LAYER_HUD);
	gameLayer->setHud(hudLayer);
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
void SingleEliminationWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (hud == NULL) hud = ((SingleElimHud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD));
	bool shouldplaySound = false;
	if (player && box == player) shouldplaySound = true;
	if (box == orderedOpponents.at(remainingGates))
	{
		//deactivate
		orderedOpponents.at(remainingGates+1)->deactivate();
		hud->boxEliminated(orderedOpponents.at(remainingGates + 1));
		//game over
		if (player && remainingGates == 0 && orderedOpponents.at(remainingGates) == player)
		{
			this->gameIsOver(true);
			return;
		}
		else if (player && orderedOpponents.at(remainingGates + 1) == player)
		{
			this->gameIsOver(false);
			return;
		}
		else if (remainingGates == 0)
		{
			shouldplaySound = true;
			this->gameIsOver(true);
		}
		if (!player) shouldplaySound = true;
	}
	if (shouldplaySound) SoundManager::getInstance()->playEffect(R_MP3_punch);
}
void SingleEliminationWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	auto scene = SingleEliminationWorld::createScene(boxesNumber, aiSmart);
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	this->replaceSceneGenereal(scene, world);
	if (carrerLevel != 0) world->setCarrierLevel(carrerLevel);
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
	if (player) inp->setSprawdzajPlayera(player);
}
