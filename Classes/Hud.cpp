#include "Hud.h"
#include "Macros.h"
#include "SingleGateWorld.h"
#include "Globals.h"
#include "MyMenu.h"
#include "Paths.h"
#include "myLayout.h"
#include "VisibleRect.h"
#include "soundManager.h"
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
	Button* pauseBtn = Button::create(R_pauseBtn, "", "", TextureResType::PLIST);
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
	infoNode = Label::create("",R_defaultFont,17);
	infoNode->setPosition(G_srodek);
	infoNode->setVisible(false);
	infoNode->setOpacity(0);
	G_enableShadow(infoNode);
	this->addChild(infoNode,-1);
	return true;
}
void Hud::pauseTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	if (touchType != Button::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	if (G_getWorld()->isPaused()) return;
	pauseNode->setVisible(true);
	float i = 0;
	for (auto node : pauseNode->getChildren())
	{
		node->setScale(0);
		auto wait = DelayTime::create(G_getFTimeScale(i));
		auto anim = EaseBackOut::create(ScaleTo::create(G_getFTimeScale(0.2f), 1));
		node->runAction(Sequence::createWithTwoActions(wait, anim));
		i += 0.1f;
	}
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->pauseGame();
}
void Hud::resumeBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	if (touchType != Button::TouchEventType::ENDED) return;
	SoundManager::getInstance()->playBtnEffect();
	float i = 0;
	for (auto node : pauseNode->getChildren())
	{
		auto wait = DelayTime::create(G_getFTimeScale(i));
		auto anim = EaseBackIn::create(ScaleTo::create(G_getFTimeScale(0.2f),0));
		node->runAction(Sequence::createWithTwoActions(wait, anim));
		i += 0.1f;
	}
	auto wait = DelayTime::create(G_getFTimeScale(0.5f));
	auto doo = CallFunc::create([this](){pauseNode->setVisible(false); });
	pauseNode->runAction(Sequence::createWithTwoActions(wait, doo));
	resumeBtnListenerExtended();
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->resumeGame();
}
void Hud::repeatBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	if (touchType != Button::TouchEventType::ENDED) return;
    SoundManager::getInstance()->stopAllEffects();
	SoundManager::getInstance()->playBtnEffect();
	Director::getInstance()->getScheduler()->setTimeScale(1);
	repeatBtnListenerExtended();
	((World*)Director::getInstance()->getRunningScene()->getChildByTag(LAYER_GAMEPLAY))->restartLevel();
}
void Hud::gotoMenuBtnListenerBase(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
	if (touchType != Button::TouchEventType::ENDED) return;
     SoundManager::getInstance()->stopAllEffects();
	SoundManager::getInstance()->playBtnEffect();
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
	str += " " + stringToDisplay;
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
				if (!G_getWorld()->hasStarted())
				{
					if (G_getWorld()->tapToContinueTapped != nullptr) G_getWorld()->tapToContinueTapped();
					return;
				}
				else player->jump();
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
	const float scaleFactor = 0.7f;
	int i = 0;
	Point position;
	const float multibtnHeight = Sprite::createWithSpriteFrameName(R_multiBtn[0])->getContentSize().height;
	const float powerUpBtnHeight = Sprite::createWithSpriteFrameName(R_powerUps[(int)type])->getContentSize().height*scaleFactor;
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
		position = Vec2(VR::rightBottom().x - powerUpBtnHeight,VR::bottom().y + powerUpBtnHeight/2.0f);
	}
	if (activatorBtns[i] != NULL) activatorBtns[i]->removeFromParent();
	activatorBtns[i] = Button::create(R_powerUps[int(type)], "", "", TextureResType::PLIST);
	activatorBtns[i]->setScale(0);
	auto scaleUp = EaseBackOut::create(ScaleTo::create(0.3f, 1.2f*scaleFactor));
	auto scaleDown = EaseBackOut::create(ScaleTo::create(0.3f, scaleFactor));
	auto idle = DelayTime::create(Director::getInstance()->getScheduler()->getTimeScale() * 2);
	activatorBtns[i]->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, idle, NULL)));
	this->addChild(activatorBtns[i],-1);
	activatorBtns[i]->addTouchEventListener([box,i, this](Ref *reff, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED) return;
		SoundManager::getInstance()->playBtnEffect();
		if (!box->activatePowerUp()) return;
		activatorBtns[i]->setTouchEnabled(false);
		activatorBtns[i]->stopAllActions();
		auto scaleDown = EaseBackIn::create(ScaleTo::create(0.3f, 0));
		auto destroy = CallFunc::create([this,i](){activatorBtns[i]->removeFromParentAndCleanup(true); activatorBtns[i] = NULL; });
		activatorBtns[i]->runAction(Sequence::createWithTwoActions(scaleDown, destroy));
	});
	activatorBtns[i]->setPosition(position);
	activatorBtns[i]->setScale(scaleFactor);
}
void Hud::displayNextLevel(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType touchType)
{
    G_displayCorrectLevelStarter(G_getWorld()->getCarrerLevel() +1,this);
}
void Hud::keyBackClickedHUD()
{
	SoundManager::getInstance()->playBtnEffect();
	if (G_getWorld()->isPaused())
	{
		this->resumeBtnListenerBase(this, cocos2d::ui::Button::TouchEventType::ENDED);
	}
	else
	{
		this->pauseTouchCallback(this, cocos2d::ui::Button::TouchEventType::ENDED);
	}
}
void Hud::addGameOverButtons(bool win,myLayout *gmOverNode)
{
    LinearLayoutParameter *param = LinearLayoutParameter::create();
    param->setGravity(LinearGravity::CENTER_VERTICAL);
    param->setMargin(cocos2d::ui::Margin(15,0,15,0));
    myLayout *btnlayout = myLayout::create();
	btnlayout->setType(1);
	btnlayout->setMargin(25, 12, 25,12);
	Button *menuBtn = Button::create(R_gotoMenuBtn, "", "", TextureResType::PLIST);
	Button *retryBtn = Button::create(R_reapeatBtn, "", "", TextureResType::PLIST);
	menuBtn->setTitleFontName(R_defaultFont);
    menuBtn->setLayoutParameter(param);
	retryBtn->setTitleFontName(R_defaultFont);
    retryBtn->setLayoutParameter(param);
	btnlayout->addWidgetCustomParam(menuBtn);
	btnlayout->addWidgetCustomParam(retryBtn);
	if (G_getWorld()->getCarrerLevel() != 0 && win)
	{
		Button *nextLevelBtn = Button::create(R_resumebtn, "", "", TextureResType::PLIST);
		nextLevelBtn->addTouchEventListener(CC_CALLBACK_2(Hud::displayNextLevel, this));
        nextLevelBtn->setLayoutParameter(param);
		btnlayout->addWidgetCustomParam(nextLevelBtn);
	}
	menuBtn->addTouchEventListener(CC_CALLBACK_2(Hud::gotoMenuBtnListenerBase, this));
	retryBtn->addTouchEventListener(CC_CALLBACK_2(Hud::repeatBtnListenerBase, this));
    //additional
    gmOverNode->addWidget(btnlayout);
    gmOverNode->setMargin(25, 25);
	gmOverNode->setAnchorPoint(Vec2(0.5, 0.5));
	gmOverNode->setBackGroundImage(R_btnOn, Widget::TextureResType::PLIST);
	gmOverNode->setPosition(G_srodek);
	gmOverNode->setOpacity(0);
	gmOverNode->runAction(FadeIn::create(0.5f*Director::getInstance()->getScheduler()->getTimeScale()));
}
