#include "EndlessHud.h"
#include "Globals.h"
#include "Paths.h"
#include "VisibleRect.h"
#include "DbReader.h"
USING_NS_CC;
using namespace ui;

bool EndlessHud::init()
{
	if (!Hud::init())
	{
		return false;
	}
	scoreText = Label::create(String::createWithFormat(G_str("score").c_str(), 0)->getCString(), R_defaultFont, G_wF(35));
	scoreText->setAnchorPoint(Vec2(0, 1));
	const float margin = 0.05*G_srodek.x;
	scoreText->setPosition(Vec2(VR::leftTop().x+margin,VR::leftTop().y- margin));
	scoreText->enableShadow();
	this->addChild(scoreText);
	return true;
}
void EndlessHud::displayGameIsOverAdditional(bool win)
{
	bool beating = false;
	bool newRecord=false;
    bool carrer = false;
    int bestSCore = DbReader::getInstance()->getEndlessBestScore();
    if (world->getCarrerLevel() !=0) carrer = true;
	if (world->getMinliczbabramek() == 0) beating = true;
	if (world->getScore() > bestSCore && beating)
	{
		DbReader::getInstance()->setEndlessBestScore(world->getScore());
		bestSCore = world->getScore();
		newRecord = true;
	}
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create("Game Over", R_defaultFont, G_wF(40));
	if (newRecord) gmOverText->setString(G_str("newRecord").c_str());
	if (win && carrer) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Completed").c_str())->getCString());
	else if (!win && carrer) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Failed").c_str())->getCString());
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, 0, 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//SCORE INFO
	if(beating)
	{
		Text *score = Text::create("SCORE:", R_defaultFont, G_wF(25));
		score->setString(String::createWithFormat(G_str("score").c_str(), world->getScore())->getCString());
		gmOverNode->addWidget(score);
		Text *bestScore = Text::create(String::createWithFormat("%s%s%d",G_str("bestScore").c_str()," : ",bestSCore)->getCString(), R_defaultFont, G_wF(25));
		gmOverNode->addWidget(bestScore);
	}
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
	if (carrer && win)
	{
		Button *nextLevelBtn = Button::create(R_resumebtn, "", "", TextureResType::PLIST);
		nextLevelBtn->addTouchEventListener(CC_CALLBACK_2(EndlessHud::displayNextLevel, this));
		btnlayout->addWidget(nextLevelBtn);
	}
	gmOverNode->setMargin(25, 25);
	gmOverNode->addWidget(btnlayout);
	gmOverNode->setAnchorPoint(Vec2(0.5, 0.5));
	gmOverNode->setBackGroundImage(R_btnOn, Widget::TextureResType::PLIST);
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
	const int x = world->getScore();
	if (world->getMinliczbabramek() != 0 && world->getMinliczbabramek() - x !=0)
	{
		displayInfo(String::createWithFormat(G_str("gatesLeft").c_str(), world->getMinliczbabramek() - x)->getCString());
	}
	else
	{
		scoreText->setString(String::createWithFormat(G_str("score").c_str(), x)->getCString());
	}
}
void EndlessHud::lateinit(World *world)
{
	this->world = (EndlessWorld*)world;
	if (this->world->getMinliczbabramek() != 0)
	{
		scoreText->setString("");
	}
}
