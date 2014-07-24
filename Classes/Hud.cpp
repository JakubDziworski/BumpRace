#include "Hud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "MyMenu.h"
#include "Paths.h"
USING_NS_CC;
using namespace ui;
bool Hud::init()
{
	if (!Layer::init())
	{
		return false;
	}
	isGameOver = false;
	//PAUSE//
	pauseNode = Node::create();
	Button* pauseBtn = Button::create("pauseBtnOn.png", "pauseBtnOf.png");
	pauseBtn->setPosition(Vec2(2 * G_srodek.x - pauseBtn->getContentSize().height*0.75f, 2 * G_srodek.y - pauseBtn->getContentSize().width*0.75f));
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(Hud::pauseTouchCallback,this));
	const int margin = G_srodek.y / 20;
	Button *resume = Button::create("resumeBtnOf.png", "resumeBtnOf.png");
	Button *repeat = Button::create("repeatBtnOn.png", "repeatBtnOn.png");
	Button *goToMenu = Button::create("gotoMenuBtnOf.png", "gotoMenuBtnOf.png");
	resume->setPositionY(resume->getContentSize().height + margin);
	goToMenu->setPositionY(-goToMenu->getContentSize().height - margin);
	resume->addTouchEventListener(CC_CALLBACK_2(Hud::resumeBtnListenerBase, this));
	repeat->addTouchEventListener(CC_CALLBACK_2(Hud::repeatBtnListenerBase, this));
	goToMenu->addTouchEventListener(CC_CALLBACK_2(Hud::gotoMenuBtnListenerBase, this));
	pauseNode->addChild(resume);
	pauseNode->addChild(repeat);
	pauseNode->addChild(goToMenu);
	pauseNode->setPosition(G_srodek);
	pauseNode->setVisible(false);
	this->addChild(pauseNode);
	this->addChild(pauseBtn, 1, B_PAUSE);
	//INFO
	infoNode = Label::create();
	infoNode->setSystemFontSize(G_wF(35));
	infoNode->setPosition(G_srodek);
	infoNode->setVisible(false);
	infoNode->setOpacity(0);
	this->addChild(infoNode);
	return true;
}
void Hud::pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	pauseNode->setVisible(true);
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->pauseGame();
}
void Hud::resumeBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	pauseNode->setVisible(false);
	resumeBtnListenerExtended();
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->resumeGame();
}
void Hud::repeatBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	Director::getInstance()->getScheduler()->setTimeScale(1);
	repeatBtnListenerExtended();
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->restartLevel();
}
void Hud::gotoMenuBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	Director::getInstance()->getScheduler()->setTimeScale(1);
	gotoMenuBtnListenerExtended();
	Director::getInstance()->replaceScene(MyMenu::createScene());
}
void Hud::displayGameOver()
{
	if (isGameOver) return;
	isGameOver = true;
	((Button*)this->getChildByTag(B_PAUSE))->setTouchEnabled(false);
	((Button*)this->getChildByTag(B_PAUSE))->runAction(FadeOut::create(0.5f*G_director->getScheduler()->getTimeScale()));
	((World*)G_dir()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->gameIsOver();
	G_dir()->getScheduler()->setTimeScale(0.1f);
	FiniteTimeAction *wait = DelayTime::create(0.1f);
	FiniteTimeAction *lategameover = CallFunc::create([&](){this->gameIsOver(); });
	this->runAction(Sequence::create(wait, lategameover, NULL));
}
void Hud::displayInfo(const std::string &stringToDisplay, Boxx* boxabout)
{
	infoNode->setVisible(true);
	std::string str = "";
	if (boxabout != NULL)
	{
		str = boxabout->getID();
	}
	str += stringToDisplay;
	infoNode->setString(str);
	FiniteTimeAction *fadein = FadeIn::create(0.2f);
	FiniteTimeAction *disable = CallFunc::create([&](){infoNode->setVisible(false); });
	FiniteTimeAction *idle = DelayTime::create(2);
	FiniteTimeAction *fadeout = FadeOut::create(0.5f);
	infoNode->runAction(Sequence::create(fadein, idle, fadeout, disable, NULL));
}
void Hud::setMultiplayer(World *world)
{
	int playerznumber = world->getPlayers()->size();
	int i = 0;
	int sizey;
	for (Player *player : *world->getPlayers())
	{
		auto btn = Button::create("multiBtn.png","");
		btn->setScale9Enabled(true);
		btn->setScaleX(2 * G_srodek.x / btn->getContentSize().width / playerznumber);
		btn->setAnchorPoint(Vec2(0, 0));
		btn->setPosition(Vec2(i*(2 * G_srodek.x / playerznumber), 0));
		btn->setColor(player->getBoxColor());
		btn->addTouchEventListener([player](Ref* pSender, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				player->jump();
			}
		});
		this->addChild(btn);
		if(i==0)sizey = btn->getContentSize().height;
		i++;
	}
	additionalMulti(sizey);
}
