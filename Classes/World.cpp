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
USING_NS_CC;
USING_NS_CC_EXT;
//----***INIT***------//
void World::lateInit()
{
	hud->lateinit(this);
	schedule(schedule_selector(World::tick));
}
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
	multiplayerEnabled = false;
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
	//auto _debugLayer = extension::PhysicsDebugNode::create(gravitySpace);
	//rotationLayer->addChild(_debugLayer, 100);
	//_debugLayer->setVisible(true);
	//****************//
	createBackground();
	createFloor();
	rozmiescCheckpointy();
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
		cpv(0,-2000),
		cpv(0, 0),
		cpv(lengtht, 0),
		cpv(lengtht, -2000),
	};
	paralexFactor = (bgImg->getContentSize().width*bgImg->getScaleX() - Director::getInstance()->getWinSize().width) / verts[3].x;
	floor = cpPolyShapeNew(floorBody, 4, verts, cpvzero);
	//SPRITE
	SpriteBatchNode *node = SpriteBatchNode::createWithTexture(SpriteFrameCache::getInstance()->getSpriteFrameByName(R_flat)->getTexture());
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_LINEAR };
	node->getTexture()->setTexParameters(tp);
	flatsprite = PhysicsSprite::createWithTexture(node->getTexture(), Rect(verts[0].x, verts[1].y, abs(verts[3].x), abs(verts[3].y)));
	flatsprite->setCPBody(floorBody);
	node->addChild(flatsprite);
	flatsprite->setAnchorPoint(Vec2(0, 1));
	rotationLayer->addChild(node);
	floor->e = 0;//elastycznosc;
	floor->u = 0.1f;//friction
	cpShapeSetLayers(floor, CPFLOORCOLIDER);
	cpSpaceAddStaticShape(gravitySpace, floor);
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
		int wysokosc = 2 * G_hF(150);//rand() % int(G_hF(600));
		int odleglosc = i +G_powerUpOdleglos+ rand() % int(G_wF(G_powerUpOdlegloscVar));
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
	bgImg->setScale(srodek.x * 4 / bgImg->getContentSize().width, srodek.y * 2 / bgImg->getContentSize().height);
	bgImg->setPosition(srodek.x, srodek.y);
	bgImg->setAnchorPoint(Vec2(0, .5f));
	this->addChild(bgImg, -1);
	scaleeLayer->setScale(srodek.x / 1280.0f);
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
	if (rotationLayer->getNumberOfRunningActions() == 0)
	{
		const int critical = rand() % 3;
		//if (critical == 0) angle = rand() % 50 - 100; else 
		angle = rand() % 60;
		const float duration = (rand() % 6 + 1) / 2.0f;
		rotationLayer->runAction(RotateTo::create(duration, angle));
	}
	changeGravity();
	checkPosition(delta);
	cameraFollowFunction();
	bgImg->setPositionX(-orderedOpponents.at(0)->getPositionX()*paralexFactor);
	customWorldUpdate();
}
void World::changeGravity()
{
	G_maxVelocity = G_maxVelConstant + G_maxVelAddition*G_mySin;
	gravitySpace->gravity = cpv(G_wF(1000) * G_mySin, G_wF(-2000) * G_myCos);
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
	for (Boxx *box : orderedOpponents)
	{
		if (box->isDeactivated()) break;
		if (box == orderedOpponents.back()){ i++; break; }
		i++;
	}
	if (i>0)
	return orderedOpponents.at(i-1);
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
	rotationLayer->pause();
	this->pause();
}
bool World::nodeOutOfWindow(Node *node)
{
	if (node->getPositionX() + 2 * srodek.x/scaleeLayer->getScale() < getOstaniActive()->getPositionX()) return true;
	return false;
}
void World::resumeGame()
{
	rotationLayer->resume();
	this->resume();
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
	this->setTouchEnabled(false);
	getHud()->displayGameOver(win);
	Director::getInstance()->getScheduler()->setTimeScale(0.1f);
	if (win && carrerLevel != 0)
	{
		DbReader::getInstance()->unlockLevel(carrerLevel + 1);
	}
}
//_________SINGLE PLAYER_________//
void World::setSinglePlayer(Player* player)
{
	//DOTYK
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(World::s_onTouched, this);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(World::s_onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);
	//PLAYERZ
	s_putOnPlayers(player);
	cameraFollowFunction = CC_CALLBACK_0(World::s_cameraFollow,this);
	lateInit();
}
bool World::s_onTouched(cocos2d::Touch* touch, cocos2d::Event* event)
{
	player->jump();
	/*if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) opponentz.at(0)->jump();
	else if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) opponentz.at(1)->jump();
	else if (keyCode == EventKeyboard::KeyCode::KEY_ALT) opponentz.at(2)->jump();*/
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
		opponent->setBodyPosition(Vec2(2 * srodek.x + losowa*(opponent->getContentSize().width*1.5f), floorBody->p.y + opponent->getContentSize().height));
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
	const float maxOffsetX = srodek.x / scaleeLayer->getScale() / screenRatio;
	const float maxOffsetY = srodek.y / scaleeLayer->getScale() / screenRatio;
	const float maxpierwszyOffset = 0.8f*srodek.y / scaleeLayer->getScale();
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
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(World::m_onTouched, this);
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(World::m_onKeyPressed, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchlistener, this);
	//PLAYERZ
	m_putOnPlayers(players);
	cameraFollowFunction = CC_CALLBACK_0(World::m_cameraFollow, this);
	lateInit();
	getHud()->setMultiplayer(this);
	this->setPositionY(Sprite::createWithSpriteFrameName(R_multiBtn)->getContentSize().height);	//podwyzszamy troche zeby przyciski nie zaslanialy nic
}
bool World::m_onTouched(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();
	if (playersNumber == 2)
	{
		if (location.x > srodek.x)
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
		if (location.x < 0.66f*srodek.x)
		{
			players.at(0)->jump();
		}
		else if (location.x < 1.32f*srodek.x)
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
		if (location.x < 0.5f*srodek.x)
		{
			players.at(0)->jump();
		}
		else if (location.x < srodek.x)
		{
			players.at(1)->jump();
		}
		else if (location.x < 1.5f*srodek.x)
		{
			players.at(2)->jump();
		}
		else
		{
			players.at(3)->jump();
		}
	}
	return true;
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
		opponent->setBodyPosition(Vec2(2 * srodek.x + losowa*(opponent->getContentSize().width*1.5f), floorBody->p.y + opponent->getContentSize().height));
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
	const float maxOffsetX = srodek.x / scaleeLayer->getScale() / screenRatio;
	const float maxOffsetY = srodek.y / scaleeLayer->getScale() / screenRatio;
	const float maxpierwszyOffset = 0.8f*srodek.y / scaleeLayer->getScale();
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
	G_dir()->replaceScene(scene);
}
