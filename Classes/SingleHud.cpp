#include "SingleHud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "Player.h"
#include "Paths.h"
#include "VisibleRect.h"
USING_NS_CC;
using namespace ui;
bool SingleGateHud::init()
{
	if (!Hud::init())
	{
		return false;
	}
	//GAME OVER VIEW//
	prevBest = NULL;
	return true;
}
void SingleGateHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	orderedBoxes = orderedByPointsBoxes;
	int i = 0;
	for (Boxx *box : *orderedByPointsBoxes)
	{
		Text* text = ((Text*)scoreTable.at(box));
		text->setString(String::createWithFormat("%s : %d", box->getID().c_str(), box->getScore())->getCString());
		if (i == 0) scoreNode->removeAllWidgets();
		scoreNode->addWidget(text);
		if (i == 0)
		{
			if (prevBest == NULL || (prevBest != box && prevBest->getScore() != box->getScore()))
			{
				text->runAction(Repeat::create(Sequence::createWithTwoActions(ScaleTo::create(0.2f, 1.1f), ScaleTo::create(0.2f, 1.0f)), 5));
				prevBest = box;
			}
		}
		i++;
	}
}
void SingleGateHud::displayGameIsOverAdditional(bool win)
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create("Game Over!", R_defaultFont, G_wF(40));
	if (world->getCarrerLevel() != 0 && win) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Completed").c_str())->getCString());
	else if (world->getCarrerLevel() != 0 && !win) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Failed").c_str())->getCString());
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//consts
	const float additionalOffset = gmOverText->getContentSize().height + margin;
	int i = 1;
	Boxx* player=NULL;
	bool ommitPlayer = false;
	if (player = G_getWorld()->getPlayer())
	{
		if (player->getScore() >= orderedBoxes->front()->getScore())
		{
			ommitPlayer = true;
			Text* text = Text::create("GATES COLLECTED", R_defaultFont, G_wF(25));
			text->enableShadow();
			text->setAnchorPoint(Vec2(0.5f, 0));
			text->setColor(player->getBoxColor());
			text->setString(String::createWithFormat("%d.%s (%d %s)", i, player->getID().c_str(), player->getScore(), G_str("GatesCollected").c_str())->getCString());
			gmOverNode->addWidget(text);
		}
	}
	for (Boxx *box : *orderedBoxes)
	{
		if (box == player && ommitPlayer) continue;
		Text* text = Text::create("GATES COLLECTED", R_defaultFont, G_wF(25));
		text->enableShadow();
		text->setAnchorPoint(Vec2(0.5f, 0));
		text->setColor(box->getBoxColor());
		text->setString(String::createWithFormat("%d.%s (%d %s)", i, box->getID().c_str(), box->getScore(),G_str("GatesCollected").c_str())->getCString());
		gmOverNode->addWidget(text);
		i++;
	}
	//**HORIZONTAL BUTTONS**//
	myLayout *btnlayout = myLayout::create();
	btnlayout->setType(1);
	btnlayout->setMargin(0, G_hF(25), 0, 0);
	Button *menuBtn = Button::create(R_gotoMenuBtn, "", "", TextureResType::PLIST);
	Button *retryBtn = Button::create(R_reapeatBtn, "", "", TextureResType::PLIST);
	menuBtn->setTitleFontName(R_defaultFont);
	retryBtn->setTitleFontName(R_defaultFont);
	btnlayout->addWidget(menuBtn);
	btnlayout->addWidget(retryBtn);
	if (world->getCarrerLevel() != 0 && win)
	{
		Button *nextLevelBtn = Button::create(R_resumebtn, "", "", TextureResType::PLIST);
		nextLevelBtn->addTouchEventListener(CC_CALLBACK_2(SingleGateHud::displayNextLevel, this));
		btnlayout->addWidget(nextLevelBtn);
	}
	gmOverNode->setMargin(25, 25);
	gmOverNode->addWidget(btnlayout);
	gmOverNode->setAnchorPoint(Vec2(0.5, 0.5));
	gmOverNode->setBackGroundImage(R_btnOn, Widget::TextureResType::PLIST);
	gmOverNode->setPosition(Vec2(G_srodek.x,2*G_srodek.y+gmOverNode->getContentSize().height));
	gmOverNode->runAction(EaseBackOut::create(MoveTo::create(0.3f*Director::getInstance()->getScheduler()->getTimeScale(), G_srodek)));
	//listeners
	menuBtn->addTouchEventListener(CC_CALLBACK_2(SingleGateHud::gotoMenuBtnListenerBase, this));
	retryBtn->addTouchEventListener(CC_CALLBACK_2(SingleGateHud::repeatBtnListenerBase, this));
	//oapcity
	scoreNode->runAction(FadeOut::create(0.5f*Director::getInstance()->getScheduler()->getTimeScale()));
	this->addChild(gmOverNode);
}
void SingleGateHud::lateinit(World *world)
{
	this->world = world;
	scoreNode = myLayout::create();
	scoreNode->setType(0);
	//TOP LEFT SCORE VIEW///
	for (Boxx *box : *world->getBoxes())
	{
		Text* text = Text::create("", R_defaultFont, G_wF(25));
		text->enableShadow();
		text->setAnchorPoint(Vec2(0, 0));
		text->setColor(box->getBoxColor());
		text->setString(String::createWithFormat("%s : 0", box->getID().c_str())->getCString());
		scoreNode->addWidget(text);
		scoreTable.insert(box, text);
	}
	scoreNode->setAnchorPoint(Vec2(0, 0));
	scoreNode->setPosition(Vec2(VR::leftBottom().x+G_srodek.x / 15,VR::leftBottom().y+ G_srodek.x / 15));
	this->addChild(scoreNode);
}
void SingleGateHud::additionalMulti(int heightY)
{
}
