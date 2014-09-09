#include "PowerUp.h"
#include "Globals.h"
#include "Paths.h"
#include "Boxx.h"
#include "Macros.h"
#include "Hud.h"
USING_NS_CC;
void PowerUp::tick(float dt)
{
	if (boxez->front()->getBoundingBox().getMaxX() < this->getBoundingBox().getMinX()) return;
	if (boxez->back()->getPositionX() > this->getPositionX() && G_getWorld()->nodeOutOfWindow(this))//out of window
	{
		this->stopAllActions();
		this->unscheduleAllSelectors();
		this->removeFromParentAndCleanup(true);
		return;
	}
	for (auto box : *boxez)
	{
		if (box->getBoundingBox().intersectsRect(this->getBoundingBox()))
		{
			if (G_getWorld()->isGameOver()) return;
			if(!box->collectedPowerUp(pwrupType)) return;
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_powerUp.c_str(), false, Director::getInstance()->getScheduler()->getTimeScale());
			((Hud*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(LAYER_HUD))->powerUpCollected(pwrupType, box);
			this->unscheduleAllSelectors();
			this->removeFromParentAndCleanup(true);
			break;
		}
	}
}
PowerUp * PowerUp::create(cocos2d::Vector<Boxx*> *boxess)
{
	PowerUp *me = new PowerUp();
	me->InitPowerUp(boxess);
	me->autorelease();
	return me;
}
bool PowerUp::InitPowerUp(cocos2d::Vector<Boxx*> *boxess)
{
    int losuj = 1;
	losuj = random(0,2);
	if (!Sprite::initWithSpriteFrameName(R_powerUps[losuj]))
	{
		return false;
	}
    this->setScale(0.7f);
    active = false;
	switch (losuj)
	{
	case 0:
		pwrupType = PowerUpType::SPEED;
		break;
	case 1:
		pwrupType = PowerUpType::GHOST;
		break;
	case 2:
		pwrupType = PowerUpType::THUNDER;
		break;
	}
	boxez = boxess;
	//auto goUp = EaseBackOut::create(MoveBy::create(0.7f, Vec2(0, G_hF(100))));
	//auto goDown = EaseBackOut::create(MoveBy::create(0.7f, Vec2(0, G_hF(-100))));
	auto goUp = EaseBackOut::create(ScaleTo::create(0.3f,0.7f));
	auto goDown = EaseBackOut::create(ScaleTo::create(0.3f, 0.8f,0.65f));
	this->runAction(RepeatForever::create(Sequence::createWithTwoActions(goUp, goDown)));
	this->schedule(schedule_selector(PowerUp::tick));
	return true;
}
