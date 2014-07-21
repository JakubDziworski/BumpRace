#define COCOS2D_DEBUG 2
#include "World.h"
#include "U.h"
#include "Boxx.h"
#include "Globals.h"
#include "Paths.h"
#include "VisibleRect.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "Checkpoint.h"
#include "CCTexture2D.h"
#include "Macros.h"
#include "Hud.h"
#include "Checkpoint.h"
USING_NS_CC;
USING_NS_CC_EXT;
bool World::myInit(int numberOfPlayers,int gates)
{
	if (!Layer::init())
	{
		return false;
	}
	//****************//
	rotationLayer = Node::create();
	moveLayer = Node::create();
	scaleeLayer = Node::create();
	moveLayer->addChild(rotationLayer);
	scaleeLayer->addChild(moveLayer);
	srodek = VisibleRect::getVisibleRect().size/2;
	scaleeLayer->setPosition(srodek);
	this->addChild(scaleeLayer,1);
	//****************//
	hud = NULL;
	boxesNumber = numberOfPlayers;
	gatesNumber = gates;
	remainingGates = gates;
	opponentz = Vector<Boxx*>(boxesNumber);
	angle = 0;
	timee = 0;
	screenRatio = Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height;
	//****************//
	gravitySpace = cpSpaceNew();
	gravitySpace->gravity = cpv(0, -2000);
	//****************//
	auto _debugLayer = extension::PhysicsDebugNode::create(gravitySpace);
	rotationLayer->addChild(_debugLayer, 100);
	_debugLayer->setVisible(true);
	//****************//
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(World::onTouched, this);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(World::onKeyPressed, this);
	createBackground();
	createFloor();
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);
	putOnPlayers();
	rozmiescCheckpointy();
	schedule(schedule_selector(World::tick));
	return true;
}

void World::createFloor()
{
	floorBody = cpBodyNew(INFINITY,INFINITY);
	float lengtht = (gatesNumber )*G_odlegloscmiedzyBramkami;
	cpVect verts[] = {
		cpv(0,-2000),
		cpv(0, 0),
		cpv(lengtht, 0),
		cpv(lengtht, -2000),
	};
	paralexFactor = (bgImg->getContentSize().width*bgImg->getScaleX() - Director::getInstance()->getWinSize().width) / verts[3].x;
	floor = cpPolyShapeNew(floorBody, 4, verts, cpvzero);
	//SPRITE
	SpriteBatchNode *node = SpriteBatchNode::create("FLAT.png");
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_LINEAR };
	node->getTexture()->setTexParameters(&tp);
	PhysicsSprite *spr = PhysicsSprite::createWithTexture(node->getTexture(), Rect(verts[0].x, verts[1].y, abs(verts[3].x), abs(verts[3].y)));
	spr->setCPBody(floorBody);
	node->addChild(spr);
	spr->setAnchorPoint(Vec2(0, 1));
	rotationLayer->addChild(node);
	floor->e = 0;//elastycznosc;
	floor->u = 0.1f;//friction
	cpSpaceAddStaticShape(gravitySpace, floor);
}
void World::tick(float delta)
{
	timee += delta;
	int steps = 2;
	float dt = Director::getInstance()->getAnimationInterval()*Director::getInstance()->getScheduler()->getTimeScale() / (float)steps;
	G_setCurrAngle(rotationLayer->getRotation());
	for (int i = 0; i < steps; i++){
		cpSpaceStep(gravitySpace, dt);
	}
	//*******************//
		if (rotationLayer->getNumberOfRunningActions() == 0)
		{
			const int critical = rand() % 3;
			//if (critical == 0) angle = rand() % 50 - 100; else 
			angle = rand()%60;
			const float duration = (rand() % 6+1)/2.0f;
			rotationLayer->runAction(RotateTo::create(duration, angle));
		}
		changeGravity();
		checkPosition(delta);
		cameraFollow(delta);
		bgImg->setPositionX(-orderedOpponents.at(0)->getPositionX()*paralexFactor);
		customWorldUpdate();
}

void World::putOnPlayers()
{
	putOnBoxes();
	float losowa;
	std::vector<float> wylosowane(boxesNumber);
	player = opponentz.at(0);
	physPosOrderedOpponentz = Vector<Boxx*>(opponentz);
	orderedOpponents = Vector<Boxx*>(opponentz);
	orderedOppByScore = Vector<Boxx*>(opponentz);
	for (Boxx *opponent : opponentz)
	{
		do
		{
			losowa = (rand() % boxesNumber)+0.1f;
		} while (std::find(wylosowane.begin(), wylosowane.end(), losowa) != wylosowane.end());
		wylosowane.push_back(losowa);
		opponent->setBodyPosition(Vec2(2*srodek.x + losowa*(opponent->getContentSize().width*1.5f), floorBody->p.y + opponent->getContentSize().height));
		rotationLayer->addChild(opponent);
	}
}
void World::changeGravity()
{
	G_maxVelocity = G_maxVelConstant + G_maxVelAddition*G_mySin;
	gravitySpace->gravity = cpv(1000 * G_mySin, -2000 * G_myCos);
}

void World::checkPosition(float dt)
{
	std::sort(orderedOpponents.begin(), orderedOpponents.end(), &World::posSortingFun);
	std::sort(physPosOrderedOpponentz.begin(), physPosOrderedOpponentz.end(), &World::physPosSortingFun);
	int i = 1;
	int j = 1;
	for (Boxx* opponent : orderedOpponents)
	{
		opponent->setRacePos(i);
		i++;
	}
	for (Boxx* opponent : physPosOrderedOpponentz)
	{
		opponent->setPhysPosition(j);
		opponent->updateBox();
		j++;
	}
}

bool World::posSortingFun(Boxx* a, Boxx* b)
{
	return (a->getPositionX() > b->getPositionX());
}

bool World::physPosSortingFun(Boxx *a, Boxx *b)
{
	if (a->isOnFlat() && b->isOnFlat())
	{
		return (a->getPositionX() > b->getPositionX());
	}
	else return false;
}

void World::putOnBoxes()
{
	for (int i = 0; i < boxesNumber; i++)
	{
		Boxx *box = Boxx::create("BOX.png", "op1", gravitySpace);
		opponentz.pushBack(box);
	}
}

void World::checkpointReachedBase(Boxx *box, int pos)
{
	if (pos == 1)
	{
		box->addPoint();
		remainingGates--;
	}
	checkpointReachedExtended(box,pos);
	if (remainingGates == 0)
	{
		if ((Hud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))
		((Hud*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))->displayGameOver();
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_MP3_punch.c_str());
}

void World::rozmiescCheckpointy()
{
	const int dlugosc = floor->bb.r - floor->bb.l;
	for (int i = 1; i <= gatesNumber; i++)
	{
		auto chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint);
		chkpt->setPosition(floor->bb.l + i*G_odlegloscmiedzyBramkami, floor->bb.t);
		rotationLayer->addChild(chkpt);
		if (i == gatesNumber) chkpt->setIsLast(true);
		modifyGate(chkpt);
	}
}


void World::createBackground()
{

	//bgImg = Sprite::createWithSpriteFrameName("tlo.png");
	bgImg = Sprite::create("tlo.png");
	bgImg->setScale(srodek.x * 4 / bgImg->getContentSize().width, srodek.y * 2 / bgImg->getContentSize().height);
	bgImg->setPosition(srodek.x,srodek.y);
	bgImg->setAnchorPoint(Vec2(0,.5f));
	this->addChild(bgImg, -1);
	scaleeLayer->setScale(srodek.x/1280.0f);
}

bool World::nodeOutOfWindow(Node *node)
{
	if (node->getPositionX() + 2*srodek.x< orderedOpponents.at(this->getBoxesNumber()-1)->getPositionX()) return true;
	return false;
}

bool World::myInitWithAI(int numberOfPlayers, int gates,int aiSmartness)
{
	this->aiSmart = aiSmartness;
	if (!myInit(numberOfPlayers, gates)) return false;
	return true;
}

cocos2d::Vector<Boxx*> * World::getSortedBoxesByScore()
{
	std::sort(orderedOppByScore.begin(), orderedOppByScore.end(), &World::scoreSortingFun);
	return &orderedOppByScore;
}

bool World::scoreSortingFun(Boxx *a, Boxx *b)	//MALEJACAO
{
	return (a->getScore() > b->getScore());
}


void World::pauseGame()
{
	rotationLayer->pause();
	this->pause();
}

void World::resumeGame()
{
	rotationLayer->resume();
	this->resume();
}
Boxx* World::getOstaniActive()
{
	int i = 0;
	for (Boxx *box : orderedOpponents)
	{
		if (box->isDeactivated()) break;
		if (box == orderedOpponents.back()){ i++; break; }
		i++;
	}
	if (i>0)
	return orderedOpponents.at(i-1);
}


void World::cameraFollow(float dt)
{
	Boxx *pierwszyy = NULL;
	if (player == orderedOpponents.at(0))
	{
		followMate = orderedOpponents.at(1);
		pierwszyy = player;
	}
	else
	{
		followMate = orderedOpponents.at(0);
		pierwszyy = followMate;
	}
	//************//
	posX = -player->getPositionX()*G_myCos;
	posY = player->getPositionX()*G_mySin;
	const float lastposX = -followMate->getPositionX()*G_myCos;
	const float lastposY = +followMate->getPositionX()*G_mySin;
	const float maxOffsetX = srodek.x / scaleeLayer->getScale() / screenRatio;
	const float maxOffsetY = srodek.y / scaleeLayer->getScale() / screenRatio;
	const float maxpierwszyOffset = 0.8f*srodek.y / scaleeLayer->getScale();
	const float pierwszyposY = pierwszyy->getPositionX()*G_mySin;
	//************//
	moveLayer->setPositionX(clampf((posX + lastposX) / 2, posX - maxOffsetX, posX + maxOffsetX));
	moveLayer->setPositionY(clampf(pierwszyposY - maxpierwszyOffset,posY-maxpierwszyOffset,posY+maxpierwszyOffset));	//TO DO CHANGE 0.8 JAKO FLAT COSTAM
}

bool World::onTouched(cocos2d::Touch* touch, cocos2d::Event* event)
{
	player->jump();
	/*if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) opponentz.at(0)->jump();
	else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) opponentz.at(1)->jump();
	else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) opponentz.at(2)->jump();*/
	return true;
}

void World::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	player->jump();
}

void World::tintToBlack()
{
}

Hud* World::getHud()
{
	if (hud == NULL)
	{
		hud = (Hud*)(G_dir()->getRunningScene()->getChildByTag(LAYER_HUD));
	}
	return hud;
}








