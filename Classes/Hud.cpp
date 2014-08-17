#include "Hud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "MyMenu.h"
#include "Paths.h"
#include "VisibleRect.h"
USING_NS_CC;
using namespace ui;
bool Hud::init()
{
	if (!Layer::init())
	{
		return false;
	}
	activatorBtn = NULL;
	isGameOver = false;
	//PAUSE//
	pauseNode = Node::create();
	Button* pauseBtn = Button::create(R_pauseBtn, R_pauseBtn, "", TextureResType::PLIST);
	pauseBtn->setTitleFontName(R_defaultFont);
	pauseBtn->setPosition(Vec2(VR::rightTop().x - pauseBtn->getContentSize().width*0.75f, VR::rightTop().y - pauseBtn->getContentSize().width*0.75f));
	pauseBtn->addTouchEventListener(CC_CALLBACK_2(Hud::pauseTouchCallback,this));
	const int margin = G_srodek.y / 20;
	Button *resume = Button::create(R_resumebtn, R_resumebtn,  "", TextureResType::PLIST);
	Button *repeat = Button::create(R_reapeatBtn, R_reapeatBtn,  "", TextureResType::PLIST);
	Button *goToMenu = Button::create(R_gotoMenuBtn, R_gotoMenuBtn,  "", TextureResType::PLIST);
	resume->setTitleFontName(R_defaultFont);
	repeat->setTitleFontName(R_defaultFont);
	goToMenu->setTitleFontName(R_defaultFont);
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
	infoNode = Label::create("",R_defaultFont,G_wF(35));
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
	if (touchType != Button::TouchEventType::ENDED) return;
	Director::getInstance()->getScheduler()->setTimeScale(1);
	gotoMenuBtnListenerExtended();
	Director::getInstance()->replaceScene(MyMenu::createScene());
}
void Hud::displayGameOver(bool win)
{
	if (isGameOver) return;
	isGameOver = true;
	((Button*)this->getChildByTag(B_PAUSE))->setTouchEnabled(false);
	((Button*)this->getChildByTag(B_PAUSE))->runAction(FadeOut::create(0.5f*G_director->getScheduler()->getTimeScale()));
	G_dir()->getScheduler()->setTimeScale(0.1f);
	FiniteTimeAction *wait = DelayTime::create(0.1f);
	FiniteTimeAction *lategameover = CallFunc::create([this,win](){this->displayGameIsOverAdditional(win); });
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
		auto btn = Button::create(R_multiBtn[0],R_multiBtn[1], "", TextureResType::PLIST);
		btn->setTitleFontName(R_defaultFont);
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
void Hud::powerUpCollected(PowerUp::PowerUpType type, Boxx* box)
{
	if (!dynamic_cast<Player*> (box)) return;
	if (G_getWorld()->isMultiplayer())
	{

	}
	else
	{
		if (activatorBtn != NULL) activatorBtn->removeFromParent();
		activatorBtn = Button::create(R_powerUps[int(type)], "", "", TextureResType::PLIST);
		activatorBtn->setScale(0);
		activatorBtn->setPosition(Vec2(G_srodek.x, VR::bottom().y + activatorBtn->getContentSize().height));
		auto scaleUp = EaseBackOut::create(ScaleTo::create(Director::getInstance()->getScheduler()->getTimeScale()*0.3f, 1.2f));
		auto scaleDown = EaseBackOut::create(ScaleTo::create(Director::getInstance()->getScheduler()->getTimeScale()*0.3f, 1));
		auto idle = DelayTime::create(Director::getInstance()->getScheduler()->getTimeScale() * 2);
		activatorBtn->runAction(RepeatForever::create(Sequence::create(scaleUp,scaleDown,idle,NULL)));
		this->addChild(activatorBtn);
		activatorBtn->addTouchEventListener([box,this](Ref *reff, Widget::TouchEventType type)
		{
			if (type != Widget::TouchEventType::ENDED) return;
			if(!box->activatePowerUp()) return;
			activatorBtn->setTouchEnabled(false);
			activatorBtn->stopAllActions();
			auto scaleDown = EaseBackIn::create(ScaleTo::create(G_getFTimeScale(0.3f), 0));
			auto destroy = CallFunc::create([this](){activatorBtn->removeFromParentAndCleanup(true); activatorBtn = NULL; });
			activatorBtn->runAction(Sequence::createWithTwoActions(scaleDown, destroy));
		});
	}
}
