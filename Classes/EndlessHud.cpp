#include "EndlessHud.h"
#include "Globals.h"
USING_NS_CC;
using namespace ui;

bool EndlessHud::init()
{
	if (!Hud::init())
	{
		return false;
	}
	scoreText = Label::create();
	scoreText->setSystemFontSize(G_wF(35));
	scoreText->setAnchorPoint(Vec2(0, 1));
	const float margin = 0.05*G_srodek.x;
	scoreText->setPosition(Vec2(margin,2*G_srodek.y - margin));
	scoreText->setString("SCORE:0");
	scoreText->enableShadow();
	this->addChild(scoreText);
	return true;
}
void EndlessHud::gameIsOver()
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create();
	gmOverText->setString("GAME OVER!");
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverText->setFontSize(40);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//SCORE INFO
	Text *score = Text::create();
	score->setFontSize(G_wF(25));
	score->setString(String::createWithFormat("SCORE:%d", world->getPlayer()->getScore())->getCString());
	gmOverNode->addWidget(score);
	//BUTTONS
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
	menuBtn->addTouchEventListener(CC_CALLBACK_2(EndlessHud::gotoMenuBtnListenerBase, this));
	retryBtn->addTouchEventListener(CC_CALLBACK_2(EndlessHud::repeatBtnListenerBase, this));
	//oapcity
	gmOverNode->setOpacity(0);
	gmOverNode->runAction(FadeIn::create(0.5f*Director::getInstance()->getScheduler()->getTimeScale()));
	this->addChild(gmOverNode);
	//obsluga zdarzenia
}
void EndlessHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	scoreText->setString(String::createWithFormat("SCORE:%d",world->getPlayer()->getScore())->getCString());
}

void EndlessHud::lateinit(World *world)
{
	this->world = world;
}
