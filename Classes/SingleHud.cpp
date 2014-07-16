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
	int i = 0;
	for (Boxx *box : *world->getBoxes())
	{
		Text* text = Text::create();
		text->setAnchorPoint(Vec2(0,0));
		if (dynamic_cast<Player*>(box)) text->setColor(Color3B(225, 50, 50));
		text->setString(String::createWithFormat("%s : 0", box->getID().c_str())->getCString());
		text->setFontSize(25);
		text->setPosition(Vec2(G_srodek.x/10, 1.1f*i));
		this->addChild(text);
		i += text->getContentSize().height;
		scoreTable.insert(box, text);
	}
	return true;
}

void SingleGateHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	int i = 0;
	for (Boxx *box : *orderedByPointsBoxes)
	{
		Text* text = ((Text*)scoreTable.at(box));
		text->setPosition(Vec2(G_srodek.x / 10, 1.1f*i));
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
