#ifndef __SINGLE_HUD_H__
#define  __SINGLE_HUD_H__
#include "Hud.h"
#include "cocos2d.h"
#include "SingleGateWorld.h"
class SingleGateHud : public Hud
{
private:
	SingleGateWorld* world;
	cocos2d::ui::Layout* scoreLayout;
	cocos2d::Map<Boxx*,cocos2d::ui::Text*> scoreTable;
public:
	virtual bool init(SingleGateWorld *worldd);
	static SingleGateHud* create(SingleGateWorld *worldd);
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);

};
#endif
