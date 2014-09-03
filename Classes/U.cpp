#include "U.h"
#include "Globals.h"
#include "World.h"
#include "Hud.h"
using namespace cocos2d;
Size U::getVisibleSize()
{
	return Director::getInstance()->getVisibleSize();
}

cocos2d::Vec2 U::getorigin()
{
	return Director::getInstance()->getVisibleOrigin();
}

cocos2d::Vec2 U::getMiddle()
{
	return Vec2(getorigin().x + getVisibleSize().width / 2.0f, getorigin().y + getVisibleSize().height / 2.0f);
}

bool U::spriteOutOfWindow(cocos2d::Sprite *spr, cocos2d::Layer *layer)
{
	if (spr->getPositionX() < -layer->getPositionX() + layer->getScaleX()*G_srodek.x * 2)
	{
		return true;
	}
	return false;
}

