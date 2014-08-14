#ifndef __U_H__
#define __U_H__

#include "cocos2d.h"
class Hud;
class World;
class U
{
public:

	static cocos2d::Size getVisibleSize();
	static cocos2d::Vec2 getorigin();
	static cocos2d::Vec2 getMiddle();
	static bool spriteOutOfWindow(cocos2d::Sprite *spr, cocos2d::Layer *layer);
};
#endif