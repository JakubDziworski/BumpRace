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
	Button* pauseBtn = Button::create("pauseBtnOn.png", "pauseBtnOf.png");
	pauseBtn->setPosition(Vec2(2 * G_srodek.x - pauseBtn->getContentSize().height*0.75f, 2 * G_srodek.y - pauseBtn->getContentSize().width*0.75f));
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(Hud::pauseTouchCallback,this));
	//pause menu//
	pauseNode = Node::create();
	const int margin = G_srodek.y / 20;
	Button *resume = Button::create("resumeBtnOf.png", "resumeBtnOf.png");
	Button *repeat = Button::create("repeatBtnOn.png", "repeatBtnOn.png");
	Button *goToMenu = Button::create("gotoMenuBtnOf.png", "gotoMenuBtnOf.png");
	resume->setPositionY(resume->getContentSize().height + margin);
	goToMenu->setPositionY(-goToMenu->getContentSize().height - margin);
	resume->addTouchEventListener(CC_CALLBACK_2(Hud::resumeBtnListener, this));
	repeat->addTouchEventListener(CC_CALLBACK_2(Hud::repeatBtnListener, this));
	goToMenu->addTouchEventListener(CC_CALLBACK_2(Hud::gotoMenuBtnListener, this));
	pauseNode->addChild(resume);
	pauseNode->addChild(repeat);
	pauseNode->addChild(goToMenu);
	pauseNode->setPosition(G_srodek);
	pauseNode->setVisible(false);
	this->addChild(pauseNode);
	this->addChild(pauseBtn);
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

void Hud::resumeBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	pauseNode->setVisible(false);
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->resumeGame();
}
void Hud::repeatBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	Director::getInstance()->getScheduler()->setTimeScale(1);
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->resumeGame();
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->restartLevel();
}

void Hud::gotoMenuBtnListener(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	Director::getInstance()->getScheduler()->setTimeScale(1);
	Director::getInstance()->replaceScene(MyMenu::createScene());
}

