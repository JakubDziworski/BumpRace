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
bool EndlessWorld::init()
{
	if (!World::myInitWithAI(4, G_endlessGateNumber, 2))
	{
		return false;
	}
	iterations = 1;
	koniec = G_endlessGateNumber*G_odlegloscmiedzyBramkami;
	dodatek = koniec;
	return true;
}
cocos2d::Scene * EndlessWorld::createScene()
{
	auto scene = Scene::create();
	auto gameLayer = EndlessWorld::create();
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = EndlessHud::create();
	scene->addChild(hudLayer, 2, LAYER_HUD);
	gameLayer->setHud(hudLayer);
	return scene;
}
void EndlessWorld::restartLevel()
{
	G_dir()->getScheduler()->setTimeScale(1);
	auto scene = EndlessWorld::createScene();
	World *world = (World*)scene->getChildByTag(LAYER_GAMEPLAY);
	this->replaceSceneGenereal(scene, world);
}
void EndlessWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	if (box != player && pos == 1)
	{
		//getHud()->displayGameOver();
	}
	getHud()->pointsChanged(&orderedOppByScore);
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
	cpSpaceAddStaticShape(gravitySpace, floor);
	flatsprite->setTextureRect(Rect(verts[0].x, verts[1].y, abs(verts[3].x), abs(verts[3].y)));
	//bg
	Sprite *bgNext = Sprite::create(R_tlo);
	bgNext->setAnchorPoint(Vec2(-1 * iterations, 0));
	if(iterations%2 == 1)bgNext->setFlippedX(true);
	bgImg->addChild(bgNext, 0, iterations);
	for (int i = koniec - dodatek; i <= koniec; i += G_odlegloscmiedzyBramkami)
	{
		auto chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint);
		chkpt->setPosition(i, floor->bb.t);
		rotationLayer->addChild(chkpt);
	}
}

