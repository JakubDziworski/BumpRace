#include "Macros.h"
#include "Globals.h"
#include "Player.h"
#include "SingleElimHud.h"
#include "Paths.h"
#include "VisibleRect.h"
#include "soundManager.h"
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
void SingleElimHud::displayGameIsOverAdditional(bool win)
{
	//general disabling
	const float margin = G_srodek.x / 15;
	gmOverNode = myLayout::create();
	gmOverNode->setType(0);
	//gmover text
	auto gmOverText = Text::create("GAME OVER!", R_defaultFont, 20);
	if (world->getCarrerLevel() != 0 && win)
	{
		gmOverText->setString(String::createWithFormat("%s %d %s", G_str("Level").c_str(), world->getCarrerLevel(), G_str("Completed").c_str())->getCString());
	}
	else if (win && !world->isMultiplayer())	//WYGRANA
	{
		gmOverText->setString(G_str("YOUWON").c_str());
	}
    else
	{
		gmOverText->setString(G_str("Eliminated").c_str());
	}
	gmOverText->setAnchorPoint(Vec2(0.5f, 0));
	LinearLayoutParameter *gameoverparam = LinearLayoutParameter::create();
	gameoverparam->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	gameoverparam->setMargin(cocos2d::ui::Margin(0,12,0,margin));
	gmOverText->setLayoutParameter(gameoverparam);
	gmOverText->setFontSize(17);
	gmOverNode->addWidgetCustomParam(gmOverText);
	//SCORETABLE
	bool ommitPlayer = false;
	if (G_getWorld()->isMultiplayer()) ommitPlayer = true;
	int i=1;
	for (unsigned j = world->getPozycje().size();j-- > 0;)
	{
        auto box = world->getPozycje().at(j);
		if (box == G_getWorld()->getPlayer() && ommitPlayer){ i++; continue; }
		Text* text = Text::create("", R_defaultFont, 12);
		G_enableShadow(text);
		text->setAnchorPoint(Vec2(0.5f, 0));
		text->setColor(box->getBoxColor());
		text->setString(String::createWithFormat("%d.%s", i, box->getID().c_str())->getCString());
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
	//ENDSCORETABLE
    this->addGameOverButtons(win,gmOverNode);
	//oapcity
	scoreNode->runAction(FadeOut::create(0.5f* Director::getInstance()->getScheduler()->getTimeScale()));
	this->addChild(gmOverNode);
	//obsluga zdarzenia
}
void SingleElimHud::lateinit(World *worldd)
{
	world = (SingleEliminationWorld*)worldd;
	scoreNode = Layout::create();
	//TOP LEFT SCORE VIEW///
	int i = 0;
	for (Boxx *box : *world->getBoxes())
	{
		Text* text = Text::create("", R_defaultFont, 12);
		G_enableShadow(text);
		text->setAnchorPoint(Vec2(0, 0));
		text->setColor(box->getBoxColor());
		text->setString(box->getID());
		text->setPositionY(1.1f*i + G_srodek.x / 15);
		scoreNode->addChild(text);
		i += text->getContentSize().height;
		scoreTable.insert(box, text);
	}
	eliminated = 0;
	scoreNode->setPosition(Vec2(VR::leftBottom().x+G_srodek.x / 15, VR::leftBottom().y+G_srodek.x / 15));
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
	//multi elim
	int i = 0;
	if (world->isMultiplayer() && dynamic_cast<Player*>(ostatni))
	{
		for (auto plyr : *world->getPlayers())
		{
			if (plyr == ostatni)
			{
				multiBtns[i]->setTouchEnabled(false);
				multiBtns[i]->runAction(TintTo::create(0.5f, 139 ,131, 120));
			}
			else i++;
		}
	}
}
void SingleElimHud::additionalMulti(int heightY)
{
	scoreNode->setPositionY(scoreNode->getPositionY() + heightY);
}


