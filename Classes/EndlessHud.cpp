#include "EndlessHud.h"
#include "Globals.h"
#include "Paths.h"
USING_NS_CC;
using namespace ui;

bool EndlessHud::init()
{
	if (!Hud::init())
	{
		return false;
	}
	scoreText = Label::create("SCORE:0",R_defaultFont,G_wF(35));
	scoreText->setAnchorPoint(Vec2(0, 1));
	const float margin = 0.05*G_srodek.x;
	scoreText->setPosition(Vec2(margin,2*G_srodek.y - margin));
	scoreText->enableShadow();
	this->addChild(scoreText);
	return true;
}
void EndlessHud::displayGameIsOverAdditional(bool win)
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create("GAME OVER!", R_defaultFont, G_wF(40));
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//SCORE INFO
	Text *score = Text::create("SCORE:", R_defaultFont, G_wF(25));
	score->setString(String::createWithFormat("SCORE:%d", world->getScore())->getCString());
	gmOverNode->addWidget(score);
	//BUTTONS
	myLayout *btnlayout = myLayout::create();
	btnlayout->setType(1);
	btnlayout->setMargin(0, G_hF(25), 0, 0);
	Button *menuBtn = Button::create(R_gotoMenuBtn, "", "", TextureResType::PLIST);
	Button *retryBtn = Button::create(R_reapeatBtn, "", "", TextureResType::PLIST);
	menuBtn->setTitleFontName(R_defaultFont);
	retryBtn->setTitleFontName(R_defaultFont);
	btnlayout->addWidget(menuBtn);
	btnlayout->addWidget(retryBtn);
	gmOverNode->setMargin(25, 25);
	gmOverNode->addWidget(btnlayout);
	gmOverNode->setAnchorPoint(Vec2(0.5, 0.5));
	gmOverNode->setBackGroundImage(R_btnOn[0], Widget::TextureResType::PLIST);
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
	int x = world->getScore();
	scoreText->setString(String::createWithFormat("SCORE:%d", world->getScore())->getCString());
	//scoreText->setString(String::createWithFormat("SCORE:%d", world->getPlayer()->getScore())->getCString());
}
void EndlessHud::lateinit(World *world)
{
	this->world = (EndlessWorld*)world;
}
