#define COCOS2D_DEBUG 2
#include "World.h"
#include "U.h"
#include "Boxx.h"
#include "Globals.h"
#include "VisibleRect.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
bool World::myInit(int numberOfPlayers)
{
	if (!Layer::init())
	{
		return false;
	}
	//****************//
	rotationLayer = Node::create();
	scaleLayer = Node::create();
	scaleLayer->setAnchorPoint(Vec2(.5, 0.5));
	//scaleLayer->setPosition(VisibleRect::getVisibleRect().origin);
	scaleLayer->addChild(rotationLayer);
	srodek = VisibleRect::getVisibleRect().size/2;
	this->addChild(scaleLayer);
	//****************//
	boxesNumber = numberOfPlayers;
	opponentz = Vector<Boxx*>(boxesNumber);
	angle = 0;
	timee = 0;
	//****************//
	gravitySpace = cpSpaceNew();
	gravitySpace->gravity = cpv(0, -2000);
	//****************//
	auto _debugLayer = extension::PhysicsDebugNode::create(gravitySpace);
	rotationLayer->addChild(_debugLayer, 100);
	_debugLayer->setVisible(true);
	//****************//
	createFloor();
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->setSwallowTouches(true);
	touchlistener->onTouchBegan = CC_CALLBACK_2(World::onTouched, this);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(World::onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	putOnPlayers();
	schedule(schedule_selector(World::tick));
	this->setScale(0.45f);
	return true;
}

void World::createFloor()
{
	floorBody = cpBodyNew(INFINITY,INFINITY);
	cpVect verts[] = {
		cpv(0,-108),
		cpv(0, 0),
		cpv(500000, 0),
		cpv(500000, -108),
	};
	floor = cpPolyShapeNew(floorBody, 4, verts, cpvzero);
	floor->e = 0;//elastycznosc;
	floor->u = 0.1f;//friction
	cpSpaceAddStaticShape(gravitySpace, floor);
}
void World::tick(float delta)
{
	timee += delta;
	int steps = 2;
	float dt = Director::getInstance()->getAnimationInterval() / (float)steps;
	G_setCurrAngle(rotationLayer->getRotation());
	for (int i = 0; i < steps; i++){
		cpSpaceStep(gravitySpace, dt);
	}
	//*******************//
		if (rotationLayer->getNumberOfRunningActions() == 0)
		{
			const int critical = rand() % 3;
			if (critical == 0) angle = rand() % 50 - 100;
			const float duration = (rand() % 6+1)/2.0f;
			angle = rand()%60;
			rotationLayer->runAction(RotateTo::create(duration, angle));
		}
		changeGravity();
		checkPosition(delta);
		cameraFollow(delta);
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
	for (Boxx *opponent : opponentz)
	{
		do
		{
			losowa = (rand() % boxesNumber) - 2.05f;
		} while (std::find(wylosowane.begin(), wylosowane.end(), losowa) != wylosowane.end());
		wylosowane.push_back(losowa);
		opponent->setBodyPosition(Vec2(srodek.x + losowa*(opponent->getContentSize().width*1.5f), floorBody->p.y + opponent->getContentSize().height));
		rotationLayer->addChild(opponent);
	}
}
void World::changeGravity()
{
	G_maxVelocity = G_maxVelConstant + G_maxVelAddition*G_mySin;
	gravitySpace->gravity = cpv(1000 * G_mySin, -2000 /** G_myCos*/);
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


