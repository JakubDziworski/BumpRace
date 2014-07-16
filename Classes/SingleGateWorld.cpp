#include "SingleGateWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Macros.h"
#include "Player.h"
#include "SingleHud.h"
#include "Checkpoint.h"
USING_NS_CC;


cocos2d::Scene* SingleGateWorld::createScene(int numberOfPlayers, int aiLevel)
{
	auto scene = Scene::create();
	auto gameLayer = SingleGateWorld::create(numberOfPlayers,aiLevel);
	scene->addChild(gameLayer, 1, LAYER_GAMEPLAY);
	auto hudLayer = SingleGateHud::create(gameLayer);
	scene->addChild(hudLayer, 2, LAYER_HUD);
	return scene;
}
bool SingleGateWorld::myInit(int numberOfPlayers, int aiLevel)
{
	if (!World::myInitWithAI(numberOfPlayers, aiLevel))
	{
		return false;
	}
	return true;
}

bool SingleGateWorld::onTouched(Touch* touch, Event* event)
{
		player->jump();
		/*if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) opponentz.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) opponentz.at(1)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) opponentz.at(2)->jump();*/
		return true;
}
void SingleGateWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	player->jump();
}

void SingleGateWorld::customWorldUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
void SingleGateWorld::cameraFollow(float dt)
{
	if (player == orderedOpponents.at(0)) followMate = orderedOpponents.at(1);
	else followMate = orderedOpponents.at(0);
		//************//
		posX = - player->getPositionX()*G_myCos;
		posY =  player->getPositionX()*G_mySin;
		const float lastposX = - followMate->getPositionX()*G_myCos;
		const float lastposY = + followMate->getPositionX()*G_mySin;
		const float maxOffsetX = srodek.x / scaleeLayer->getScale()/screenRatio;
		const float maxOffsetY = srodek.y / scaleeLayer->getScale()/screenRatio;
		//************//
		moveLayer->setPositionX(clampf((posX + lastposX) / 2.0f, posX - maxOffsetX, posX+maxOffsetX));
		moveLayer->setPositionY(clampf((posY + lastposY) / 2.0f, posY - maxOffsetY, posY + maxOffsetY));
}
SingleGateWorld* SingleGateWorld::create(int numberOfPlayers, int aiLevel)
{
	SingleGateWorld *pRet = new SingleGateWorld();
	if (pRet && pRet->myInit(numberOfPlayers,aiLevel))
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

void SingleGateWorld::putOnBoxes()
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

void SingleGateWorld::rozmiescCheckpointy()
{
		const int dlugosc = floor->bb.r - floor->bb.l;
		for (int i = 4000; i < dlugosc; i += 4000)
		{
			auto *chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint);
			chkpt->setPosition(floor->bb.l + i, floor->bb.t);
			rotationLayer->addChild(chkpt,-1);
		}
}

void SingleGateWorld::checkpointReachedExtended(Boxx *box, int pos)
{
	((SingleGateHud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))->pointsChanged(getSortedBoxesByScore());
}


