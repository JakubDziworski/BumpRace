#include "EndlessWorld.h"
#include "Globals.h"
#include "EndlessHud.h"
#include "Macros.h"
#include "Checkpoint.h"
#include "extensions/cocos-ext.h"
#include "Paths.h"
#include "external/chipmunk/include/chipmunk/chipmunk_unsafe.h"
USING_NS_CC_EXT;
USING_NS_CC;
bool EndlessWorld::init(int oppNum,int aiLevel)
{
	if (!World::myInitWithAI(oppNum, G_endlessGateNumber, aiLevel))
	{
		return false;
	}
	iterations = 1;
	koniec = G_endlessGateNumber*G_odlegloscmiedzyBramkami;
	dodatek = koniec;
	minliczbabramek = 0;
	score = 0;
	return true;
}
cocos2d::Scene * EndlessWorld::createScene(int oppNum,int aiLevel)
{
	auto scene = Scene::create();
	auto gameLayer = EndlessWorld::create(oppNum, aiLevel);
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = EndlessHud::create();
	scene->addChild(hudLayer, 2, LAYER_HUD);
	gameLayer->setHud(hudLayer);
	return scene;
}
void EndlessWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	auto scene = EndlessWorld::createScene(boxesNumber,aiSmart);
	EndlessWorld *world = (EndlessWorld*)scene->getChildByTag(LAYER_GAMEPLAY);
	this->replaceSceneGenereal(scene, world);
	world->setMinGates(minliczbabramek);
	if (carrerLevel != 0) world->setCarrierLevel(carrerLevel);
}
void EndlessWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (box == player && pos == boxesNumber)
	{
		box->deactivate();
		G_getWorld()->gameIsOver(false);
		return;
	}
	if (box == player)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_MP3_punch.c_str(), false, Director::getInstance()->getScheduler()->getTimeScale());
		score++;
		box->positiveGateAction();
		getHud()->pointsChanged(&orderedOppByScore);
	}
	if (score == minliczbabramek && minliczbabramek != 0)
	{
		G_getWorld()->gameIsOver(true);
		return;
	}
	remainingGates++;
}
void EndlessWorld::shouldEnableSlowmo(Chcekpoint *chkpt, bool first)
{
	chkpt->enableSlowmo();
}
void EndlessWorld::customWorldUpdate()
{
	if (orderedOpponents.at(0)->getPositionX() + 2 * G_srodek.x > koniec)	//zblizamy sie do konca drogi trzeba dodac nowe rzeczy
	{
		koniec += dodatek;
		extendFlat();
		iterations++;
	}
}
void EndlessWorld::extendFlat()
{
	//flat
	cpVect verts[] = {
		cpv(0, -2000),
		cpv(0, 0),
		cpv(koniec, 0),
		cpv(koniec, -2000),
	};
	cpSpaceRemoveStaticShape(gravitySpace,floor);
	floor = cpPolyShapeNew(floorBody, 4, verts, cpvzero);
	floor->e = 0;//elastycznosc;
	floor->u = 0.1f;//friction
	floor->collision_type = COLLISIONTYPEFLOOR;
	cpSpaceAddStaticShape(gravitySpace, floor);
	flatsprite->setTextureRect(Rect(verts[0].x, verts[1].y, abs(verts[3].x), abs(verts[3].y)));
	//bg
	Sprite *bgNext = Sprite::createWithSpriteFrameName(R_tlo);
	bgNext->setAnchorPoint(Vec2(-1.0f * iterations, 0));
	bgNext->setPositionX(bgNext->getPositionX() - 2*iterations);
	if(iterations%2 == 1)bgNext->setFlippedX(true);
	bgImg->addChild(bgNext, 0, iterations);
	for (int i = koniec - dodatek; i <= koniec; i += G_odlegloscmiedzyBramkami)
	{
		auto chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint);
		chkpt->setPosition(i, floor->bb.t);
		rotationLayer->addChild(chkpt);
	}
	cpShapeSetLayers(floor, CPFLOORCOLIDER);
}
void EndlessWorld::modifyGate(Chcekpoint *chkpt)
{
	chkpt->setSprawdzajPierwszych(false);
}
EndlessWorld * EndlessWorld::create(int oppNum, int aiLevel)
{
	{
		EndlessWorld *pRet = new EndlessWorld();
		if (pRet && pRet->init(oppNum, aiLevel))
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
}
void EndlessWorld::replaceSceneAdditional(cocos2d::Scene *scene,World *world)
{
	((EndlessWorld*)world)->setMinGates(((EndlessWorld*)world)->getMinliczbabramek());
}
