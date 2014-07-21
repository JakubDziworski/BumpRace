#include "EndlessWorld.h"
#include "Globals.h"
#include "EndlessHud.h"
#include "Macros.h"
#include "Checkpoint.h"
USING_NS_CC;
bool EndlessWorld::init()
{
	if (!World::myInitWithAI(4, 5, 2))
	{
		return false;
	}
	return true;
}

cocos2d::Scene * EndlessWorld::createScene()
{
	auto scene = Scene::create();
	auto gameLayer = EndlessWorld::create();
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = EndlessHud::create(gameLayer);
	scene->addChild(hudLayer, 2, LAYER_HUD);
	return scene;
}

void EndlessWorld::restartLevel()
{
	G_dir()->replaceScene(EndlessWorld::createScene());
}

void EndlessWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (box == player && pos != 1)
	{
		getHud()->displayGameOver();
	}
}

void EndlessWorld::shouldEnableSlowmo(Chcekpoint *chkpt, bool first)
{
	chkpt->enableSlowmo();
}

