#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"

class VR
{
public:
    static cocos2d::Rect getVisibleRect();
	static void setShouldLazyInit(bool val);
	static cocos2d::Vec2 left();
    static cocos2d::Vec2 right();
    static cocos2d::Vec2 top();
    static cocos2d::Vec2 bottom();
    static cocos2d::Vec2 center();
    static cocos2d::Vec2 leftTop();
    static cocos2d::Vec2 rightTop();
    static cocos2d::Vec2 leftBottom();
    static cocos2d::Vec2 rightBottom();
private:
	VR();
    static void lazyInit();
	static bool shouldLazyInit;
    static cocos2d::Rect s_visibleRect;
};

#endif /* __VISIBLERECT_H__ */
