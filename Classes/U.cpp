#include "U.h"
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
