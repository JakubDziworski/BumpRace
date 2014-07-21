#include "Hud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "MyMenu.h"
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
	infoNode->setSystemFontSize(G_wF(25));
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

void Hud::pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes)
{

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

void Hud::gameIsOver()
{

}

void Hud::displayGameOver()
{
	if (isGameOver) return;
	isGameOver = true;
	//((Button*)pauseNode->getChildByTag(B_PAUSE))->setTouchEnabled(false);
	//((Button*)pauseNode->getChildByTag(B_PAUSE))->runAction(FadeOut::create(0.5f*G_director->getScheduler()->getTimeScale()));
	G_dir()->getScheduler()->setTimeScale(0.1f);
	FiniteTimeAction *wait = DelayTime::create(0.1f);
	FiniteTimeAction *lategameover = CallFunc::create([&](){this->gameIsOver(); });
	this->infoNode->runAction(Sequence::create(wait, lategameover, NULL));
}

void Hud::displayInfo(const std::string &stringToDisplay)
{
	infoNode->setString(stringToDisplay);
	infoNode->setVisible(true);
	FiniteTimeAction *fadein = FadeIn::create(0.2f);
	FiniteTimeAction *disable = CallFunc::create([&](){infoNode->setVisible(false); });
	FiniteTimeAction *idle = DelayTime::create(2);
	FiniteTimeAction *fadeout = FadeOut::create(0.5f);
	infoNode->runAction(Sequence::create(fadein, idle, fadeout, disable, NULL));
}

