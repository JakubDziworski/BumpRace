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
	for (int i = 0; i < 4; i++)
	{
		activatorBtns[i] = NULL;
		multiBtns[i] =  NULL;
	}
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
	FiniteTimeAction *wait = DelayTime::create(0.4f);
	FiniteTimeAction *lategameover = CallFunc::create([this,win](){this->displayGameIsOverAdditional(win); });
	this->runAction(Sequence::create(wait, lategameover, NULL));
	//power UpS buttons and multibuttons hide
	float i = 0;
	for (auto btn : multiBtns)
	{
		if (btn == NULL) continue;
		btn->setTouchEnabled(false);
		auto wait = DelayTime::create(i*Director::getInstance()->getScheduler()->getTimeScale());
		auto move = EaseBackIn::create(MoveBy::create(0.4f*Director::getInstance()->getScheduler()->getTimeScale(), Vec2(0, -btn->getContentSize().height)));
		btn->runAction(Sequence::createWithTwoActions(wait, move));
		i += 0.4f;
	}
	i = 0;
	for (auto pwrupBtn : activatorBtns)
	{
		if (pwrupBtn == NULL) continue;
		pwrupBtn->setTouchEnabled(false);
		auto wait = DelayTime::create(i*Director::getInstance()->getScheduler()->getTimeScale());
		auto move = EaseBackIn::create(MoveTo::create(0.4f*Director::getInstance()->getScheduler()->getTimeScale(), Vec2(VR::left().x-pwrupBtn->getContentSize().width, pwrupBtn->getPositionY())));
		pwrupBtn->runAction(Sequence::createWithTwoActions(wait, move));
		i += 0.2f;
	}

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
		multiBtns[i] = Button::create(R_multiBtn[0], R_multiBtn[1], "", TextureResType::PLIST);
		multiBtns[i]->setTitleFontName(R_defaultFont);
		multiBtns[i]->setScale9Enabled(true);
		multiBtns[i]->setScaleX(2 * G_srodek.x / multiBtns[i]->getContentSize().width / playerznumber);
		multiBtns[i]->setAnchorPoint(Vec2(0, 0));
		multiBtns[i]->setPosition(Vec2(i*(2 * G_srodek.x / playerznumber), 0));
		multiBtns[i]->setColor(player->getBoxColor());
		multiBtns[i]->addTouchEventListener([player](Ref* pSender, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				player->jump();
			}
		});
		this->addChild(multiBtns[i]);
		if(i==0)sizey = multiBtns[i]->getContentSize().height;
		i++;
	}
	additionalMulti(sizey);
}
void Hud::powerUpCollected(PowerUp::PowerUpType type, Boxx* box)
{
	if (!dynamic_cast<Player*> (box)) return;
	int i = 0;
	Point position;
	const float multibtnHeight = Sprite::createWithSpriteFrameName(R_multiBtn[0])->getContentSize().height;
	const float powerUpBtnHeight = Sprite::createWithSpriteFrameName(R_powerUps[(int)type])->getContentSize().height;
	if (G_getWorld()->isMultiplayer())
	{
		for (Player *player : *G_getWorld()->getPlayers())
		{
			if (player == box) break;
			i++;
		}
		const float dzialka = VR::right().x / G_getWorld()->getPlayers()->size();
		position = Point((i+0.5f)*dzialka, VR::bottom().y + multibtnHeight + 0.5f*powerUpBtnHeight);
	}
	else
	{
		position = Vec2(G_srodek.x, VR::bottom().y + powerUpBtnHeight);
	}
	if (activatorBtns[i] != NULL) activatorBtns[i]->removeFromParent();
	activatorBtns[i] = Button::create(R_powerUps[int(type)], "", "", TextureResType::PLIST);
	activatorBtns[i]->setScale(0);
	auto scaleUp = EaseBackOut::create(ScaleTo::create(Director::getInstance()->getScheduler()->getTimeScale()*0.3f, 1.2f));
	auto scaleDown = EaseBackOut::create(ScaleTo::create(Director::getInstance()->getScheduler()->getTimeScale()*0.3f, 1));
	auto idle = DelayTime::create(Director::getInstance()->getScheduler()->getTimeScale() * 2);
	activatorBtns[i]->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, idle, NULL)));
	this->addChild(activatorBtns[i]);
	activatorBtns[i]->addTouchEventListener([box,i, this](Ref *reff, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED) return;
		if (!box->activatePowerUp()) return;
		activatorBtns[i]->setTouchEnabled(false);
		activatorBtns[i]->stopAllActions();
		auto scaleDown = EaseBackIn::create(ScaleTo::create(G_getFTimeScale(0.3f), 0));
		auto destroy = CallFunc::create([this,i](){activatorBtns[i]->removeFromParentAndCleanup(true); activatorBtns[i] = NULL; });
		activatorBtns[i]->runAction(Sequence::createWithTwoActions(scaleDown, destroy));
	});
	activatorBtns[i]->setPosition(position);
}
