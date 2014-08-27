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
    
	scoreText = Label::createWithBMFont(R_bmfont,String::createWithFormat(G_str("score").c_str(), 0)->getCString(),17);
	scoreText->setAnchorPoint(Vec2(0, 1));
	const float margin = 0.05*G_srodek.x;
	scoreText->setPosition(Vec2(VR::leftTop().x+margin,VR::leftTop().y- margin));
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
	auto gmOverText = TextBMFont::create("Game Over", R_bmfont, 20);
	if (newRecord) gmOverText->setString(G_str("newRecord").c_str());
	if (win && carrer) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Completed").c_str())->getCString());
	else if (!win && carrer) gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Failed").c_str())->getCString());
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(Margin(0, G_wF(25), 0, margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//SCORE INFO
	if(beating)
	{
		auto score = TextBMFont::create("SCORE:", R_bmfont,12);
		score->setString(String::createWithFormat(G_str("score").c_str(), world->getScore())->getCString());
		gmOverNode->addWidget(score);
		auto bestScore = TextBMFont::create(String::createWithFormat("%s%s%d", G_str("bestScore").c_str(), " : ", bestSCore)->getCString(), R_bmfont,12);
		gmOverNode->addWidget(bestScore);
	}
    this->addGameOverButtons(win,gmOverNode);
	this->addChild(gmOverNode);
	//obsluga zdarzenia
}
void EndlessHud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{
	const int x = world->getScore();
	if (world->getMinliczbabramek() != 0 && world->getMinliczbabramek() - x !=0)
	{
        const int remainingGates = world->getMinliczbabramek() - x;
        if(remainingGates%10 > 4)
            G_getHud()->displayInfo(String::createWithFormat(G_str("gatesLeft").c_str(), remainingGates)->getCString());
        else if(remainingGates%10 > 1)
            G_getHud()->displayInfo(String::createWithFormat(G_str("gatesLeft2").c_str(), remainingGates)->getCString());
        else if(remainingGates == 1)
            G_getHud()->displayInfo(G_str("lastGate"));;
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
