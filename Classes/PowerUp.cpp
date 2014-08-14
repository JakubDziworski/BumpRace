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
	if (boxez->back()->getPositionX() > this->getPositionX() + 3*G_srodek.x)//out of window
	{
		//TODO uncomment
		//this->removeFromParentAndCleanup(true);
	}
	for (auto box : *boxez)
	{
		if (box->getBoundingBox().intersectsRect(this->getBoundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(R_MP3_punch.c_str(), false, Director::getInstance()->getScheduler()->getTimeScale());
			box->collectedPowerUp(pwrupType);
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
	active = false;
	int losuj = 0;
	losuj = 1;// rand() % 1;
	if (!Sprite::initWithSpriteFrameName(R_powerUps[losuj]))
	{
		return false;
	}
	PowerUpType type;
	switch (losuj)
	{
	case 0:
		pwrupType = PowerUpType::SPEED;
		break;
	case 1:
		pwrupType = PowerUpType::GHOST;
		break;
	}
	boxez = boxess;
	auto goUp = EaseBackOut::create(MoveBy::create(0.7f, Vec2(0, G_hF(100))));
	auto goDown = EaseBackOut::create(MoveBy::create(0.7f, Vec2(0, G_hF(-100))));
	this->runAction(RepeatForever::create(Sequence::createWithTwoActions(goUp, goDown)));
	this->schedule(schedule_selector(PowerUp::tick));
	return true;
}
