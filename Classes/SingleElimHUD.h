#ifndef __SINGLE_ELIM_HUD_H__
#define  __SINGLE_ELIM_HUD_H__
#include "Hud.h"
#include "cocos2d.h"
#include "SingleEliminationWorld.h"
#include "myLayout.h"
class SingleElimHud : public Hud
{
private:
	SingleEliminationWorld* world;
	cocos2d::Map<Boxx*,cocos2d::ui::Text*> scoreTable;
	cocos2d::ui::Layout *scoreNode;
	int eliminated;
	myLayout *gmOverNode;
public:
	virtual bool init();
	CREATE_FUNC(SingleElimHud);
	virtual void displayGameIsOverAdditional(bool win);
	virtual void boxEliminated(Boxx* ostatni);
	virtual void lateinit(World *world);
	virtual void additionalMulti(int heightY);

};
#endif
