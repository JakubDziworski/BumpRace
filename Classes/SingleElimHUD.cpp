#include "Macros.h"
#include "Globals.h"
#include "Player.h"
#include "SingleElimHud.h"
#include "Paths.h"
USING_NS_CC;
using namespace ui;
bool SingleElimHud::init()
{
	if (!Hud::init())
	{
		return false;
	}
	
	return true;
}
void SingleElimHud::gameIsOver()
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create("GAME OVER!", R_defaultFont, G_wF(40));
	if (dynamic_cast<Player*>(world->getOstaniActive()))	//WYGRANA
	{
		gmOverText->setString("WYGRANA!");
	}
	else
	{
		gmOverText->setString("ELIMINATED!");
	}
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverText->setFontSize(40);
	gmOverNode->addWidgetCustomParam(gmOverText);
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
	menuBtn->addTouchEventListener(CC_CALLBACK_2(SingleElimHud::gotoMenuBtnListenerBase, this));
	retryBtn->addTouchEventListener(CC_CALLBACK_2(SingleElimHud::repeatBtnListenerBase, this));
	//oapcity
	scoreNode->runAction(FadeOut::create(0.5f* Director::getInstance()->getScheduler()->getTimeScale()));
	gmOverNode->setOpacity(0);
	gmOverNode->runAction(FadeIn::create(0.5f*Director::getInstance()->getScheduler()->getTimeScale()));
	this->addChild(gmOverNode);
	//obsluga zdarzenia
}
void SingleElimHud::lateinit(World *worldd)
{
	world = worldd;
	scoreNode = Layout::create();
	//TOP LEFT SCORE VIEW///
	int i = 0;
	for (Boxx *box : *world->getBoxes())
	{
		Text* text = Text::create("", R_defaultFont, G_wF(25));
		text->setAnchorPoint(Vec2(0, 0));
		if (dynamic_cast<Player*>(box)) text->setColor(Color3B(225, 50, 50));
		text->setString(box->getID());
		text->setPositionY(1.1f*i + G_srodek.x / 15);
		scoreNode->addChild(text);
		i += text->getContentSize().height;
		scoreTable.insert(box, text);
	}
	eliminated = 0;
	scoreNode->setPosition(Vec2(G_srodek.x / 15, G_srodek.x / 15));
	this->addChild(scoreNode);
	//GAME OVER VIEW//
}
void SingleElimHud::boxEliminated(Boxx* ostatni)
{

	this->displayInfo("ELIMINATED!",ostatni);
	Text *wyeliminowany = scoreTable.at(ostatni);
	const float zwolnienie = G_dir()->getScheduler()->getTimeScale();
	auto powieksz = ScaleTo::create(0.35f*zwolnienie, 1.4f);
	auto pomniejsz = ScaleTo::create(0.35f*zwolnienie, 1);
	wyeliminowany->runAction(Sequence::createWithTwoActions(powieksz, pomniejsz));
	wyeliminowany->setColor(Color3B(100, 100, 100));
	eliminated++;
}

void SingleElimHud::additionalMulti(int heightY)
{
	scoreNode->setPositionY(scoreNode->getPositionY() + heightY);
}


