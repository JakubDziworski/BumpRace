#include "CarrerWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
#include "Paths.h"
#include "Checkpoint.h"
USING_NS_CC;


cocos2d::Scene* CarrerWorld::createScene(int numberOfPlayers)
{
	auto scene = Scene::create();
	auto layer = CarrerWorld::create(numberOfPlayers);
	scene->addChild(layer);
	return scene;
}
bool CarrerWorld::myInit(int numberOfPlayers)
{
	if (!World::myInit(numberOfPlayers))
	{
		return false;
	}
	return true;
}

bool CarrerWorld::onTouched(Touch* touch, Event* event)
{
		player->jump();
		/*if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) opponentz.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) opponentz.at(1)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) opponentz.at(2)->jump();*/
		return true;
}
void CarrerWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	player->jump();
}

void CarrerWorld::customWorldUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
void CarrerWorld::cameraFollow(float dt)
{
	if (player == orderedOpponents.at(1)) followMate = orderedOpponents.at(2);
	else followMate = orderedOpponents.at(1);
		//************//
		posX = - player->getPositionX()*G_myCos;
		posY =  player->getPositionX()*G_mySin;
		const float maxOffsetX = 2*srodek.x * G_myCos;
		const float maxOffsetY = srodek.x  * G_mySin;
		//************//
		const float lastposX = - followMate->getPositionX()*G_myCos;
		const float lastposY = + followMate->getPositionX()*G_mySin;
		moveLayer->setPositionX(posX - clampf((posX - lastposX) / 2.0, -maxOffsetX, maxOffsetX));
		moveLayer->setPositionY(posY + clampf((posY - lastposY) / 2.0, -maxOffsetY, maxOffsetY));
		moveLayer->setPositionY(moveLayer->getPositionY() + 3.0f*G_Currangle);
}
CarrerWorld* CarrerWorld::create(int numberOfPlayers)
{
	CarrerWorld *pRet = new CarrerWorld();
	if (pRet && pRet->myInit(numberOfPlayers))
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

void CarrerWorld::putOnBoxes()
{
	opponentz.pushBack(Boxx::create("BOX.png", "KUBA", gravitySpace));
	player = opponentz.at(0);
	for (int i = 1; i < boxesNumber; i++)
	{
		auto aiop = AIOpponent::create("BOX.png", CCString::createWithFormat("AI_%d",i)->getCString(), gravitySpace);
		opponentz.pushBack(aiop);
		aiop->addOrderedOpponents(orderedOpponents);
	}
}

void CarrerWorld::rozmiescCheckpointy()
{
		const int dlugosc = floor->bb.r - floor->bb.l;
		for (int i = 40000; i < dlugosc; i += 40000)
		{
			auto *chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint, true);
			chkpt->setPosition(floor->bb.l + i, floor->bb.t);
			rotationLayer->addChild(chkpt);
		}
}



