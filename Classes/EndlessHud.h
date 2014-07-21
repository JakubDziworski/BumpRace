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
	myLayout *gmOverNode;
public:
	virtual bool init(EndlessWorld* worldd);
	static EndlessHud* create(EndlessWorld* worldd);
	virtual void gameIsOver();

};
#endif // !___ENDLESS_HUD_H_
