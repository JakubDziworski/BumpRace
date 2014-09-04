#ifndef ___ENDLESS_HUD_H_
#define ___ENDLESS_HUD_H_
#include "Hud.h"
#include "EndlessWorld.h"
#include "myLayout.h"
class EndlessHud : public Hud
{
private:
	EndlessWorld* world;
	cocos2d::Vector<Boxx*> *orderedBoxes;
	cocos2d::Label *scoreText;
	myLayout *gmOverNode;
public:
	virtual bool init();
	CREATE_FUNC(EndlessHud);
	virtual void displayGameIsOverAdditional(bool win);
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	virtual void lateinit(World *world);

};
#endif // !___ENDLESS_HUD_H_
