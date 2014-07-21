#include "SingleHud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "Player.h"
USING_NS_CC;
using namespace ui;
bool SingleGateHud::init(SingleGateWorld *worldd)
{
	if (!Hud::init())
	{
		return false;
	}
	world = worldd;
	scoreNode = Layout::create();
	//TOP LEFT SCORE VIEW///
	int i = 0;
	for (Boxx *box : *world->getBoxes())
	{
		Text* text = Text::create();
		text->setAnchorPoint(Vec2(0,0));
		if (dynamic_cast<Player*>(box)) text->setColor(Color3B(225, 50, 50));
		text->setString(String::createWithFormat("%s : 0", box->getID().c_str())->getCString());
		text->setFontSize(25);
		text->setPositionY(1.1f*i + G_srodek.x / 15);
		scoreNode->addChild(text);
		i += text->getContentSize().height;
		scoreTable.insert(box, text);
	}
	scoreNode->setPosition(Vec2(G_srodek.x / 15, G_srodek.x / 15));
	this->addChild(scoreNode);
	//GAME OVER VIEW//
	return true;
}

void SingleGateHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	orderedBoxes = orderedByPointsBoxes;
	int i = 0;
	for (Boxx *box : *orderedByPointsBoxes)
	{
		Text* text = ((Text*)scoreTable.at(box));
		text->setPositionY (G_srodek.x / 15);
		text->setString(String::createWithFormat("%s : %d", box->getID().c_str(), box->getScore())->getCString());
		i += text->getContentSize().height;
	}
}

SingleGateHud* SingleGateHud::create(SingleGateWorld *worldd)
{
	SingleGateHud *pRet = new SingleGateHud();
	if (pRet && pRet->init(worldd))
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

void SingleGateHud::gameIsOver()
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create();
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverText->setString("Game Over");
	gmOverText->setFontSize(40);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//consts
	const float additionalOffset = gmOverText->getContentSize().height + margin;
	int i = 1;
	for (Boxx *box : *orderedBoxes)
	{
		Text* text = Text::create();
		text->setAnchorPoint(Vec2(0.5f, 0));
		if (dynamic_cast<Player*>(box)) text->setColor(Color3B(225, 50, 50));
		text->setString(String::createWithFormat("%d.%s(%d gates collected)", i, box->getID().c_str(), box->getScore())->getCString());
		text->setFontSize(G_wF(25));
		gmOverNode->addWidget(text);
		i++;
	}
	//**HORIZONTAL BUTTONS**//
	myLayout *btnlayout = myLayout::create();
	btnlayout->setType(1);
	btnlayout->setMargin(0, G_hF(25), 0, 0);
	Button *menuBtn = Button::create("gotoMenuBtnOf.png");
	Button *retryBtn = Button::create("repeatBtnOn.png");
	btnlayout->addWidget(menuBtn);
	btnlayout->addWidget(retryBtn);
	gmOverNode->setMargin(25, 25);
	gmOverNode->addWidget(btnlayout);
	gmOverNode->setAnchorPoint(Vec2(0.5, 0.5));
	gmOverNode->setBackGroundImage("btnOn.png");
	gmOverNode->setPosition(G_srodek);
	//listeners
	menuBtn->addTouchEventListener(CC_CALLBACK_2(SingleGateHud::gotoMenuBtnListenerBase, this));
	retryBtn->addTouchEventListener(CC_CALLBACK_2(SingleGateHud::repeatBtnListenerBase, this));
	//oapcity
	scoreNode->runAction(FadeOut::create(0.5f* Director::getInstance()->getScheduler()->getTimeScale()));
	gmOverNode->setOpacity(0);
	gmOverNode->runAction(FadeIn::create(0.5f*Director::getInstance()->getScheduler()->getTimeScale()));
	this->addChild(gmOverNode);
}



