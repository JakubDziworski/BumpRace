#include "SingleHud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "Player.h"
#include "Paths.h"
#include "VisibleRect.h"
#include "soundManager.h"
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
	auto gmOverText = Text::create("Game Over!", R_defaultFont, 20);
    gmOverText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	if (world->getCarrerLevel() != 0 && win) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Completed").c_str())->getCString());
	else if (world->getCarrerLevel() != 0 && !win) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Failed").c_str())->getCString());
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
     gameoverparam->setMargin(cocos2d::ui::Margin(0,12,0,0));
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
			Text* text = Text::create("GATES COLLECTED", R_defaultFont, 12);
			G_enableShadow(text);
			text->setAnchorPoint(Vec2(0.5f, 0));
			text->setColor(player->getBoxColor());
			text->setString(String::createWithFormat("%d.%s (%d %s)", i, player->getID().c_str(), player->getScore(), G_str("GatesCollected").c_str())->getCString());
			gmOverNode->addWidget(text);
		}
	}
	for (Boxx *box : *orderedBoxes)
	{
		if (box == player && ommitPlayer){ i++; continue; }
		Text* text = Text::create("GATES COLLECTED", R_defaultFont, 12);
		G_enableShadow(text);
		text->setAnchorPoint(Vec2(0.5f, 0));
		text->setColor(box->getBoxColor());
		text->setString(String::createWithFormat("%d.%s (%d %s)", i, box->getID().c_str(), box->getScore(),G_str("GatesCollected").c_str())->getCString());
		gmOverNode->addWidget(text);
		i++;
	}
	if (world->getCarrerLevel() != 0 && win)
	{
		auto shareScore = Button::create(R_fbshareBtn, "", "", cocos2d::ui::Widget::TextureResType::PLIST);
		shareScore->addTouchEventListener([this](Ref *reff, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			SoundManager::getInstance()->playBtnEffect();
			FB_shareLevelCompletedPost(world->getCarrerLevel());
		});
		gmOverNode->addWidget(shareScore);
	}
	this->addGameOverButtons(win,gmOverNode);
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
		Text* text = Text::create("", R_defaultFont, 12);
		G_enableShadow(text);
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
    scoreNode->setPositionY(scoreNode->getPositionY()+heightY);
}
