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
	return true;
}

bool SingleEliminationWorld::onTouched(Touch* touch, Event* event)
{
		player->jump();
		/*if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) opponentz.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) opponentz.at(1)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) opponentz.at(2)->jump();*/
		return true;
}
void SingleEliminationWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	player->jump();
}

void SingleEliminationWorld::customWorldUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
void SingleEliminationWorld::cameraFollow(float dt)
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
	if (box == orderedOpponents.at(remainingGates+1))
	{
		box->deactivate();
	}
	((SingleElimHud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))->pointsChanged(getSortedBoxesByScore());
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
