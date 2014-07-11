#include "CarrerWorld.h"
#include "Globals.h"
#include "AIOpponent.h"
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
		posX = srodek.x - player->getPositionX()*G_myCos;
		posY = srodek.y*0.75f + player->getPositionX()*G_mySin;
		const float maxOffsetX = 2*srodek.x * G_myCos;
		const float maxOffsetY = srodek.x  * G_mySin;
		//************//
		const float lastposX = srodek.x - followMate->getPositionX()*G_myCos;
		const float lastposY = srodek.y + followMate->getPositionX()*G_mySin;
		scaleLayer->setPositionX(posX - clampf((posX - lastposX) / 2.0, -maxOffsetX, maxOffsetX));
		scaleLayer->setPositionY(posY + clampf((posY - lastposY) / 2.0, -maxOffsetY, maxOffsetY));
		scaleLayer->setPositionY(scaleLayer->getPositionY() + 3.0f*G_Currangle);
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
	opponentz.pushBack(Boxx::create("BOX.png", "ja", gravitySpace));
	player = opponentz.at(0);
	for (int i = 1; i < boxesNumber; i++)
	{
		auto aiop = AIOpponent::create("BOX.png", "Opponent", gravitySpace);
		opponentz.pushBack(aiop);
		aiop->addOrderedOpponents(orderedOpponents);
	}
}



