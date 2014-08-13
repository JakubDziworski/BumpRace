#ifndef __SINGLE_HUD_H__
#define  __SINGLE_HUD_H__
#include "Hud.h"
#include "cocos2d.h"
#include "SingleGateWorld.h"
#include "myLayout.h"
class SingleGateHud : public Hud
{
private:
	World* world;
	cocos2d::Map<Boxx*,cocos2d::ui::Text*> scoreTable;
	cocos2d::Vector<Boxx*> *orderedBoxes;
	myLayout *scoreNode;
	myLayout *gmOverNode;
public:
	virtual bool init();
	CREATE_FUNC(SingleGateHud);
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	virtual void displayGameIsOverAdditional(bool win);
	virtual void lateinit(World *world);
	virtual void additionalMulti(int heightY);

};
#endif
