#include "World.h"
#include "U.h"
#include "Boxx.h"
#include "Globals.h"
#include "Paths.h"
#include "VisibleRect.h"
#include "extensions/cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "Checkpoint.h"
#include "Macros.h"
#include "Hud.h"
#include "Checkpoint.h"
#include "Player.h"
#include "AIOpponent.h"
#include "DbReader.h"
#include "PowerUp.h"
#include "soundManager.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
//----***INIT***------//
void World::lateInit()
{
	hud->lateinit(this);
	rozmiescCheckpointy();
	schedule(schedule_selector(World::tick));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f), CallFunc::create([this](){startBoxPointer(); })));
	//keyBack listener
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyReleased = CC_CALLBACK_2(World::onKeyReleased, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keylistener, this);
	//collison handling
	cpSpaceAddCollisionHandler(gravitySpace, COLLISONTYPEBOX, COLLISONTYPEBOX, NULL, NULL, World::boxesCollided, NULL, NULL);
	cpSpaceAddCollisionHandler(gravitySpace, COLLISONTYPEBOX, COLLISIONTYPEFLOOR, NULL, NULL, World::boxFeltDown, NULL, NULL);
	SoundManager::getInstance()->playBgMusicGameplay();
}
bool World::myInit(int numberOfPlayers,int gates)
{
	if (!Layer::init())
	{
		return false;
	}
	//*stop music*//
	SoundManager::getInstance()->fadeOutMusic();
	//****************//
	rotationLayer = Node::create();
	moveLayer = Node::create();
	scaleeLayer = Node::create();
	moveLayer->addChild(rotationLayer);
	scaleeLayer->addChild(moveLayer);
	scaleeLayer->setPosition(G_srodek);
	this->addChild(scaleeLayer,1);
	//****************//
	hud = NULL;
	//CLOUDSNODES
	cloudsNode = Node::create();
	cloudsNode->setPosition(Vec2(0, 0));
	cloudsNode->setAnchorPoint(Vec2(0, 0));
	this->addChild(cloudsNode);
	//
	gameOver = false;
	multiplayerEnabled = false;
	boxesNumber = numberOfPlayers;
	gatesNumber = gates;
	remainingGates = gates;
	opponentz = Vector<Boxx*>(boxesNumber);
	angle = 0;
	timee = 0;
	screenRatio = G_srodek.x / G_srodek.y;
	//****************//
	gravitySpace = cpSpaceNew();
	gravitySpace->gravity = cpv(0, -2000);
	//****************//
	//auto _debugLayer = extension::PhysicsDebugNode::create(gravitySpace);
	//rotationLayer->addChild(_debugLayer, 100);
	//_debugLayer->setVisible(true);
	//****************//
	createBackground();
	createFloor();
//	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) this->setScale(0.5f);
	return true;
}
bool World::myInitWithAI(int numberOfPlayers, int gates, int aiSmartness)
{
	this->aiSmart = aiSmartness;
	if (!myInit(numberOfPlayers, gates)) return false;
	return true;
}
//-----**CREATE**------//
void World::createFloor()
{
	floorBody = cpBodyNew(INFINITY,INFINITY);
	float lengtht = (gatesNumber + 1)*G_odlegloscmiedzyBramkami;
	cpVect verts[] = {
		cpv(0,-650),
		cpv(0, 0),
		cpv(lengtht, 0),
		cpv(lengtht, -650),
	};
	paralexFactor = (bgImg->getContentSize().width*bgImg->getScaleX() - VR::right().x) / verts[3].x;
	floor = cpPolyShapeNew(floorBody, 4, verts, cpvzero);
	//SPRITE
	floorBatchNode = SpriteBatchNode::create(R_flatTop.c_str());
	Texture2D::TexParams tp = { GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_REPEAT };
	floorBatchNode->getTexture()->setTexParameters(tp);
	flatsprite = PhysicsSprite::createWithTexture(floorBatchNode->getTexture(), Rect(verts[0].x, verts[1].y, abs(verts[3].x), floorBatchNode->getTexture()->getContentSize().height));
	flatsprite->setCPBody(floorBody);
	floorBatchNode->addChild(flatsprite);
	flatsprite->setAnchorPoint(Vec2(0, 1));
	//FLATSPRITE BOTTOM
	auto flatSPriteBottom = SpriteBatchNode::create(R_flatBottom.c_str());
	flatSPriteBottom->getTexture()->setTexParameters(tp);
	bottomSpr = Sprite::createWithTexture(flatSPriteBottom->getTexture(), Rect(0, 0, abs(verts[3].x), 2500));
	bottomSpr->setAnchorPoint(Vec2(0, 1));
	bottomSpr->setPosition(flatsprite->getBoundingBox().getMinX(), flatsprite->getBoundingBox().getMinY());
	rotationLayer->addChild(floorBatchNode);
	rotationLayer->addChild(bottomSpr);
	floor->e = 0;//elastycznosc;
	floor->u = 0.1f;//friction
	floor->collision_type = COLLISIONTYPEFLOOR;
	cpShapeSetLayers(floor, CPFLOORCOLIDER);
	cpSpaceAddStaticShape(gravitySpace, floor);
	//drzewko
}
void World::rozmiescCheckpointy()
{
	for (int i = 1; i <= gatesNumber; i++)
	{
		auto chkpt = Chcekpoint::create(this, &orderedOpponents, R_SPRITE_checkpoint);
		chkpt->setPosition(floor->bb.l + i*G_odlegloscmiedzyBramkami, floor->bb.t);
		rotationLayer->addChild(chkpt);
		if (i == gatesNumber) chkpt->setIsLast(true);
		modifyGate(chkpt);
	}
	//POWER UPY
	int i = 0;
	do
	{
		int wysokosc = 2.5f * Sprite::createWithSpriteFrameName(R_Box[1])->getContentSize().height;
		int odleglosc = i +G_powerUpOdleglos+ rand() % int(G_powerUpOdlegloscVar);
		PowerUp *pwrup = PowerUp::create(&orderedOpponents);
		pwrup->setPosition(Vec2(odleglosc, floor->bb.t + wysokosc));
		i = odleglosc;
		rotationLayer->addChild(pwrup);
	} 
	while (i < floor->bb.r);
}
void World::createBackground()
{

	bgImg = Sprite::createWithSpriteFrameName(R_tlo);
	bgImg->setScale(VR::right().x * 2.0f / bgImg->getContentSize().width, VR::top().y / bgImg->getContentSize().height);
	bgImg->setPosition(VR::leftBottom());
	bgImg->setAnchorPoint(Vec2(0, 0));
	this->addChild(bgImg, -2);
	//DPIscaleFactor = (512.0f / 106.f) / (Director::getInstance()->getOpenGLView()->getFrameSize().width / (float)Device::getDPI());
	DPIscaleFactor = clampf((512.0f / 106.f) / (Director::getInstance()->getOpenGLView()->getFrameSize().width / (float)Device::getDPI()),0.45,0.9f);
	scaleeLayer->setScale(DPIscaleFactor);
}
//----****UPDATE****----///
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
	if (rotationLayer->getNumberOfRunningActions() == 0 && !gameOver)
	{
		angle = rand() % 70;
		const float duration = (rand() % 6 + 1) / 2.0f;
		rotationLayer->runAction(RotateTo::create(duration, angle));
		calculateSredniaPredkoscDoDzwieku();
	}
	changeGravity();
	checkPosition(delta);
	if(cameraFollowFunction != nullptr) cameraFollowFunction();
	if (cameraFollowFunction != nullptr)
	{
		bgImg->setPositionX(-orderedOpponents.at(0)->getPositionX()*paralexFactor);
	}
	if (started)
	{
			generateClouds();
			generateClouds();
			generateDrzewka();
			timee = 0;
	}
	customWorldUpdate();
}
void World::changeGravity()
{
	G_maxVelocity = G_maxVelConstant + G_maxVelAddition*G_mySin;
	gravitySpace->gravity = cpv(800.0f * G_mySin,-500.0f * G_myCos);
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
void World::checkpointReachedBase(Boxx *box, int pos)
{
	if (pos == 1)
	{
		remainingGates--;
	}
	checkpointReachedExtended(box, pos);
}
//----****SORTING FUNCTIONS***-----//
cocos2d::Vector<Boxx*> * World::getSortedBoxesByScore()
{
	std::sort(orderedOppByScore.begin(), orderedOppByScore.end(), &World::scoreSortingFun);
	return &orderedOppByScore;
}
bool World::scoreSortingFun(Boxx *a, Boxx *b)	//MALEJACAO
{
	return (a->getScore() > b->getScore());
}
Boxx* World::getOstaniActive()
{
	int i = 0;
	int lowest=0;
	for (Boxx *box : orderedOpponents)
	{
		if (!box->isDeactivated()) { lowest = i; }
		i++;
	}
	return orderedOpponents.at(lowest);
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
//----****MISC***-----//
void World::pauseGame()
{
	paused = true;
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	rotationLayer->pause();
	this->setKeyboardEnabled(false);
	this->pause();
	for (auto cloud : cloudsNode->getChildren())
	{
		cloud->pause();
	}
}
bool World::nodeOutOfWindow(Node *node)
{
	auto parent = node->getParent();
	const Point pointUpperRight = parent->convertToWorldSpace(Point(node->getBoundingBox().getMaxX(), node->getBoundingBox().getMaxY()));
	const Point pointLowerLeft = parent->convertToWorldSpace(Point(node->getBoundingBox().getMinX(), node->getBoundingBox().getMinY()));
	const Point pointLowerRight = parent->convertToWorldSpace(Point(node->getBoundingBox().getMaxX(), node->getBoundingBox().getMinY()));
	const Point pointUpperLeft = parent->convertToWorldSpace(Point(node->getBoundingBox().getMinX(), node->getBoundingBox().getMaxY()));
	const Rect screenRect = VR::getVisibleRect();

	if (pointUpperRight.x > screenRect.getMinX() || pointLowerRight.x > screenRect.getMinX())
	return false;
	return true;
}
void World::resumeGame()
{
	if (!started)
	{
		this->setKeyboardEnabled(true);
		getEventDispatcher()->resumeEventListenersForTarget(this);
		return;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
	rotationLayer->resume();
	this->resume();
	for (auto cloud : cloudsNode->getChildren())
	{
		cloud->resume();
	}
	paused = false;
}
Hud* World::getHud()
{
	if (hud == NULL)
	{
		hud = (Hud*)(G_dir()->getRunningScene()->getChildByTag(LAYER_HUD));
	}
	return hud;
}
void World::gameIsOver(bool win)
{
	if (gameOver) return;
	gameOver = true;
	this->setTouchEnabled(false);
	Director::getInstance()->getScheduler()->setTimeScale(0.1f);
	SoundManager::getInstance()->enableSlowMo();
	getHud()->displayGameOver(win);
	if (win && carrerLevel != 0)
	{
		DbReader::getInstance()->unlockLevel(carrerLevel + 1);
	}
	rotationLayer->stopAllActions();
	Director::getInstance()->getScheduler()->setTimeScale(0.1f);
	const float offset1 = 2*G_srodek.y / scaleeLayer->getScale();
	const float guUp = offset1 * G_myCos;
	const float goRight = offset1*G_mySin;
	const float velocitty = 2250.0f;
	auto delay = DelayTime::create(0.3f);
	auto stopCamera = CallFunc::create([this](){cameraFollowFunction = nullptr; SoundManager::getInstance()->disableSlowMo(); Director::getInstance()->getScheduler()->setTimeScale(1);});
	auto moveToEnd = MoveTo::create(offset1 / velocitty, Vec2(moveLayer->getPositionX() - goRight, moveLayer->getPositionY() - guUp));
	auto stopSimulation = CallFunc::create([this]{this->unscheduleAllSelectors();  SoundManager::getInstance()->stopSlideEffect(); });
	moveLayer->runAction(Sequence::create(delay, stopCamera, moveToEnd, stopSimulation, NULL));
	//sound
	SoundManager::getInstance()->gameIsOver(win);
}
Boxx* World::getPrzedOstaniActive()
{
	int i = 0;
	int ostatni = 0;
	int przedostatni = 0;
	for (Boxx *box : orderedOpponents)
	{
		if (!box->isDeactivated()) { przedostatni = ostatni; ostatni = i; }
		i++;
	}
	return orderedOpponents.at(przedostatni);
}
//_________SINGLE PLAYER_________//f
void World::setSinglePlayer(Player* player)
{
	//DOTYK
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(World::s_onTouched, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);
	//PLAYERZ
	s_putOnPlayers(player);
	cameraFollowFunction = CC_CALLBACK_0(World::s_cameraFollow,this);
	lateInit();
}
bool World::s_onTouched(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!started){ if(tapToContinueTapped != nullptr) tapToContinueTapped(); return true; }
	if(player) player->jump();
	return true;
}
void World::s_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	player->jump();
}
void World::s_putOnPlayers(Player* playerr)
{
	bool used[6] = { false, false, false, false, false, false };
	player = playerr;
	for (int i = 0; i < 6; i++)
	{
		if (player->getBoxColor() == G_colors[i])
			used[i] = true;
	}
	opponentz.pushBack(player);
	for (int i = 1; i < boxesNumber; i++)
	{
		int wolny;
		for (int j = 0; j < 6; j++)
		{
			if (used[j] == false)
			{
				wolny = j;
				used[j] = true;
				break;
			}
		}
		auto aiop = AIOpponent::create(R_Box[wolny], String::createWithFormat("%s %d", G_str("Opponent").c_str(), i)->getCString(), gravitySpace, aiSmart, G_colors[wolny]);
		opponentz.pushBack(aiop);
		aiop->addOrderedOpponents(orderedOpponents);
	}
	float losowa;
	std::vector<float> wylosowane(boxesNumber);
	physPosOrderedOpponentz = Vector<Boxx*>(opponentz);
	orderedOpponents = Vector<Boxx*>(opponentz);
	orderedOppByScore = Vector<Boxx*>(opponentz);
	for (Boxx *opponent : opponentz)
	{
		do
		{
			losowa = (rand() % boxesNumber) + 0.1f;
		} while (std::find(wylosowane.begin(), wylosowane.end(), losowa) != wylosowane.end());
		wylosowane.push_back(losowa);
		opponent->setBodyPosition(Vec2(2.0f* G_srodek.x /scaleeLayer->getScale() + losowa*(opponent->getContentSize().width*2.0f), floorBody->p.y + opponent->getContentSize().height*0.5f));
		rotationLayer->addChild(opponent);
	}
}
void World::s_cameraFollow()
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
	const float maxOffsetX = G_srodek.x / scaleeLayer->getScale() / screenRatio;
	const float maxOffsetY = G_srodek.y / scaleeLayer->getScale() / screenRatio;
	const float maxpierwszyOffset = 0.8f*G_srodek.y / scaleeLayer->getScale();
	const float pierwszyposY = pierwszyy->getPositionX()*G_mySin;
	//************//
	moveLayer->setPositionX(clampf((posX + lastposX) / 2, posX - maxOffsetX, posX + maxOffsetX));
	moveLayer->setPositionY(clampf(pierwszyposY - maxpierwszyOffset, posY - maxpierwszyOffset, posY));	//TO DO CHANGE 0.8 JAKO FLAT COSTAM
}
//_______MULTIPLAYER__________//
void World::setMultiplayer(cocos2d::Vector<Player*> players)
{
	//DOTYK
	multiplayerEnabled = true;
	playersNumber = players.size();
	auto touchlistener = EventListenerTouchAllAtOnce::create();
	touchlistener->onTouchesBegan = CC_CALLBACK_2(World::m_onTouched, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);
	//PLAYERZ
	m_putOnPlayers(players);
	cameraFollowFunction = CC_CALLBACK_0(World::m_cameraFollow, this);
	lateInit();
	getHud()->setMultiplayer(this);
	scaleeLayer->setPositionY(scaleeLayer->getPositionY()+Sprite::createWithSpriteFrameName(R_multiBtn[0])->getContentSize().height);	//podwyzszamy troche zeby przyciski nie zaslanialy nic
}
void World::m_onTouched(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (!started){ tapToContinueTapped(); }
	for (auto touch : touches)
	{
		auto location = touch->getLocation();
		if (playersNumber == 2)
		{
			if (location.x > G_srodek.x)
			{
				players.at(1)->jump();
			}
			else
			{
				players.at(0)->jump();
			}
		}
		else if (playersNumber == 3)
		{
			if (location.x < 0.66f*G_srodek.x)
			{
				players.at(0)->jump();
			}
			else if (location.x < 1.32f*G_srodek.x)
			{
				players.at(1)->jump();
			}
			else
			{
				players.at(2)->jump();
			}
		}
		else if (playersNumber == 4)
		{
			if (location.x < 0.5f*G_srodek.x)
			{
				players.at(0)->jump();
			}
			else if (location.x < G_srodek.x)
			{
				players.at(1)->jump();
			}
			else if (location.x < 1.5f*G_srodek.x)
			{
				players.at(2)->jump();
			}
			else
			{
				players.at(3)->jump();
			}
		}
	}
}
void World::m_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (playersNumber > 3)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_0) players.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) players.at(1)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) players.at(2)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) players.at(3)->jump();
	}
	else if (playersNumber > 2)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_0) players.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) players.at(1)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) players.at(2)->jump();
	}
	else if (playersNumber > 1)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_0) players.at(0)->jump();
		else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) players.at(1)->jump();
	}
	else if (playersNumber > 0)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_0) players.at(0)->jump();
	}
}
void World::m_putOnPlayers(cocos2d::Vector<Player*> players)
{
	bool used[6] = { false, false, false, false, false, false };
	player = NULL;
	for (auto player : players)
	{
		for (int i = 0; i < 6; i++)
		{
			if (player->getBoxColor() == G_colors[i])
				used[i] = true;
		}
		opponentz.pushBack(player);
		this->players.pushBack(player);
	}
	for (int j = 1; j <= boxesNumber-playersNumber; j++)
	{
		int wolny;
		for (int i = 0; i < 6; i++)
		{
			if (used[i] == false)
			{
				wolny = i;
				used[i] = true;
				break;
			}
		}
		AIOpponent *opp = AIOpponent::create(R_Box[wolny], String::createWithFormat("%s %d", G_str("Opponent").c_str(),j)->getCString(), gravitySpace, aiSmart, G_colors[wolny]);
		opp->addOrderedOpponents(orderedOpponents);
		opponentz.pushBack(opp);
	}
	float losowa;
	std::vector<float> wylosowane(boxesNumber);
	physPosOrderedOpponentz = Vector<Boxx*>(opponentz);
	orderedOpponents = Vector<Boxx*>(opponentz);
	orderedOppByScore = Vector<Boxx*>(opponentz);
	for (Boxx *opponent : opponentz)
	{
		do
		{
			losowa = (rand() % boxesNumber) + 0.1f;
		} while (std::find(wylosowane.begin(), wylosowane.end(), losowa) != wylosowane.end());
		wylosowane.push_back(losowa);
		opponent->setBodyPosition(Vec2(2.0f* G_srodek.x / scaleeLayer->getScale() + losowa*(opponent->getContentSize().width*2.0f), floorBody->p.y + opponent->getContentSize().height*0.5f));
		rotationLayer->addChild(opponent);
	}
}
void World::m_cameraFollow()
{
	Boxx *pierwszyy = NULL;	//PIERWSZY PLAYER
	int i = 0;
	do
	{
		pierwszyy = orderedOpponents.at(i);
		i++;
	}
	while (dynamic_cast<Player*> (pierwszyy) == NULL);
	Boxx *ostatni = orderedOpponents.at(boxesNumber-1);
	//************//
	posX = -pierwszyy->getPositionX()*G_myCos;
	posY = pierwszyy->getPositionX()*G_mySin;
	const float lastposX = -ostatni->getPositionX()*G_myCos;
	const float lastposY = +ostatni->getPositionX()*G_mySin;
	const float maxOffsetX = G_srodek.x / scaleeLayer->getScale() / screenRatio;
	const float maxOffsetY = G_srodek.y / scaleeLayer->getScale() / screenRatio;
	const float maxpierwszyOffset = 0.8f*G_srodek.y / scaleeLayer->getScale();
	const float pierwszyposY = pierwszyy->getPositionX()*G_mySin;
	//************//
	moveLayer->setPositionX(clampf((posX + lastposX) / 2, posX - maxOffsetX, posX + maxOffsetX));
	moveLayer->setPositionY(clampf(pierwszyposY - maxpierwszyOffset, posY - maxpierwszyOffset, posY));	//TO DO CHANGE 0.8 JAKO FLAT COSTAM
}
void World::replaceSceneGenereal(Scene *scene,World *world)
{
	G_dir()->getScheduler()->setTimeScale(1);
	if (multiplayerEnabled)
	{
		Vector<Player*> plyrz;
		for (auto plyr : players)
		{
			plyrz.pushBack(Player::create(plyr->getFileName(), plyr->getID(), world->getGravitySpace(),plyr->getBoxColor()));
		}
		world->setMultiplayer(plyrz);
	}
	else
	{
		world->setSinglePlayer(Player::create(player->getFileName(), player->getID(), world->getGravitySpace(),player->getBoxColor()));
	}
	replaceSceneAdditional(scene,world);
	G_dir()->replaceScene(scene);
}
void World::calculateSredniaPredkoscDoDzwieku()
{
	float suma = 0;
	const float defaultSpeed = 400.0f;
	for (auto child : orderedOpponents)
	{
		suma+=child->getVelocityX();
	}
	const float srednia = suma / boxesNumber;
	SoundManager::getInstance()->playSlideEffect(clampf(srednia / defaultSpeed, 0.5f, 1.6f));
}
void World::onExit()
{
	Node::onExit();
	Director::getInstance()->getScheduler()->setTimeScale(1); 
	SoundManager::getInstance()->disableSlowMo(); 
	SoundManager::getInstance()->stopSlideEffect();
}
void World::startBoxPointer()
{
	auto startBtn = Label::create(G_str("tapToStart"), R_defaultFont, 17);
	G_enableShadow(startBtn);
	hud->addChild(startBtn, -1);
	startBtn->setPosition(VR::center());
	startBtn->setScale(0);
	auto show = ScaleTo::create(0.2f,0.9f, 1.2f);
	auto lateSequence = CallFunc::create([startBtn](){startBtn->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.3f, 1), ScaleBy::create(0.3f, 0.9f, 1.2f)))); });
	startBtn->runAction(Sequence::createWithTwoActions(show, lateSequence));
	auto labels = cocos2d::Vector<Label*>(boxesNumber);
	float i = 0;
	for (auto box : opponentz)
	{
		auto label = Label::create(box->getID(), R_defaultFont, 27);
		box->addChild(label);
		label->setAnchorPoint(Vec2(0.5f, 0));
		label->setNormalizedPosition(Vec2(0.5f, 1.3f));
		label->setScale(0);
		label->setColor(box->getBoxColor());
		G_enableShadow(label);
		CallFunc *blink = NULL;
		if (dynamic_cast<Player*>(box))
		{
			blink = CallFunc::create([label](){label->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(0.3f, Vec2(0, 25.0f)), MoveBy::create(0.3f, Vec2(0, -25.0f))))); });
		}
		else
		{
			blink = CallFunc::create([label](){label->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(0.3f, Vec2(0,2.5f)), MoveBy::create(0.3f, Vec2(0, -2.5f))))); });
		}
		auto show = ScaleTo::create(0.4f, 1);
		auto wait = DelayTime::create(i);
		label->runAction(Sequence::create(wait,show,blink,NULL));
		labels.pushBack(label);
		i+=0.2f;
	}
	tapToContinueTapped = [startBtn, labels,this]()
	{
		started = true;
		G_getWorld()->resumeGame();
		for (auto label : labels)
		{
			startBtn->stopAllActions();
			auto remv = CallFunc::create([startBtn, labels]{for (auto lab : labels) lab->removeFromParent(); startBtn->removeFromParent(); });
			startBtn->runAction(EaseBackIn::create(ScaleTo::create(0.2f, 0)));
			label->runAction(Sequence::createWithTwoActions(FadeOut::create(1), remv));
		}
		generateDrzewka();
		generateClouds();
	};
	rotationLayer->pause();
	this->pause();
	getEventDispatcher()->resumeEventListenersForTarget(this);
}
void World::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		G_getHud()->keyBackClickedHUD();
	}
}
void World::boxesCollided(cpArbiter *arb, cpSpace *space, void *unused)
{
	if (!cpArbiterIsFirstContact(arb)) return;
	const cpVect impulse = cpArbiterTotalImpulse(arb);
	if (abs(impulse.x) + abs(impulse.y) > 55.0f)
	SoundManager::getInstance()->playEffect(R_boxColided);
}
void World::boxFeltDown(cpArbiter *arb, cpSpace *space, void *unused)
{
	if (!cpArbiterIsFirstContact(arb)) return;
	const cpVect impulse = cpArbiterTotalImpulse(arb);
	if (abs(impulse.x) + abs(impulse.y) > 100.0f)
		SoundManager::getInstance()->playEffect(R_boxFelt);
}
void World::generateClouds()
{
	if (!pierwszeChmurki)
	{
		for (int i = 0; i<7; i++)
		{
			const int wielkosc = rand() % (5) + 1;
			auto chmurka = Sprite::createWithSpriteFrameName(String::createWithFormat(R_cloudsFormat.c_str(), wielkosc)->getCString());
			const float randPosY = VR::top().y*(float(rand() % 70 + 20) / 100.0f);
			const float randPosX = VR::right().x*(float(rand() % 100) / 100.0f);
			const float szybkosc = (15 + float(rand() % 100) / 5.0f)*(randPosX / VR::right().x);
			//chmurka modifaction
			auto delay = DelayTime::create((float(rand() % 100)) / 150.0f);
			chmurka->setPosition(randPosX, randPosY);
			chmurka->setScale(0.01f);
			chmurka->runAction(Sequence::createWithTwoActions(MoveTo::create(szybkosc, Vec2(VR::left().x - chmurka->getContentSize().width, randPosY)), CallFunc::create([chmurka](){chmurka->removeFromParent(); })));
			chmurka->runAction(Sequence::createWithTwoActions(delay, CallFunc::create([chmurka]()
			{
				chmurka->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.3f, 0.9f, 1.1f), ScaleTo::create(1.1f, 1))));
			})));
			if (rand() % 2) chmurka->setFlippedX(true);
			cloudsNode->addChild(chmurka, wielkosc);
		}
		pierwszeChmurki = true;
	}
	if (cloudsNode->getChildren().size() > 5) return;
	const int wielkosc = rand() % (5) + 1;
	const float szybkosc = 9 + float(rand() % 100) / 20.0f;
	auto chmurka = Sprite::createWithSpriteFrameName(String::createWithFormat(R_cloudsFormat.c_str(), wielkosc)->getCString());
	float randPosY = VR::top().y*(float(rand() % 70 + 20) / 100.0f);
	//chmurka modifaction
	auto delay = DelayTime::create((float(rand()%100))/150.0f);
	chmurka->setPosition(VR::right().x + chmurka->getContentSize().width, randPosY);
	chmurka->setScale(0.01f);
	chmurka->runAction(Sequence::create(delay, MoveTo::create(szybkosc, Vec2(VR::left().x - chmurka->getContentSize().width, randPosY)), CallFunc::create([chmurka](){chmurka->removeFromParent(); }), NULL));
	chmurka->runAction(Sequence::createWithTwoActions(delay, CallFunc::create([chmurka]()
	{
		chmurka->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.3f, 0.9f, 1.1f), ScaleTo::create(1.1f, 1))));
	})));
	if (rand() % 2) chmurka->setFlippedX(true);
	cloudsNode->addChild(chmurka, wielkosc);
}
void World::generateDrzewka()
{
	if (rotationLayer->convertToWorldSpace(Point(dlugoscDrzewekDuzych, 0)).x < VR::right().x + 25)
	{
		const int j = -2;
		auto spr = Sprite::createWithSpriteFrameName(R_drzewka.c_str());
		rotationLayer->addChild(spr, j);
		spr->setAnchorPoint(Vec2(0, 0));
		spr->setScale((float(rand() % 550 + 80)) / 100.0f);
		if (rotationLayer->convertToWorldSpace(Point(dlugoscDrzewekDuzych, 0)).x > VR::right().x+15)
			spr->setPosition(dlugoscDrzewekDuzych - rand() % 15, 0);//no animation
		else
		{
			spr->setPosition(dlugoscDrzewekDuzych - rand() % 15,-spr->getBoundingBox().getMaxY());//no animation
			spr->runAction(Sequence::createWithTwoActions(DelayTime::create(-j*0.2f + 0.8f), MoveBy::create(0.3f, Vec2(0, -spr->getBoundingBox().getMinY()))));
		}
		if (rand() % 2) spr->setFlippedX(true);
		dlugoscDrzewekDuzych = spr->getBoundingBox().getMaxX();
	}
	if (rotationLayer->convertToWorldSpace(Point(dlugoscDrzewekMalych, 0)).x < VR::right().x + 25)
	{
		const int j = -1;
		auto spr = Sprite::createWithSpriteFrameName(R_drzewka.c_str());
		rotationLayer->addChild(spr, j);
		spr->setAnchorPoint(Vec2(0, 0));
		spr->setScale((float(rand() % 120 + 80)) / 100.0f);
		if (rotationLayer->convertToWorldSpace(Point(dlugoscDrzewekMalych, 0)).x > VR::right().x + 15)
			spr->setPosition(dlugoscDrzewekMalych - rand() % 15, 0);//no animation
		else
		{
			spr->setPosition(dlugoscDrzewekMalych - rand() % 15, -spr->getBoundingBox().getMaxY());//no animation
			spr->runAction(Sequence::createWithTwoActions(DelayTime::create(-j*0.2f +0.8f), MoveBy::create(0.3f, Vec2(0, -spr->getBoundingBox().getMinY()))));
		}
		if (rand() % 2) spr->setFlippedX(true);
		dlugoscDrzewekMalych = spr->getBoundingBox().getMaxX();

	}
}
