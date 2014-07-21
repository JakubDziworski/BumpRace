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
	cocos2d::Vector<Boxx*> *orderedBoxes;
	cocos2d::ui::Layout *scoreNode;
	myLayout *gmOverNode;
	void lateGameIsOver();
public:
	virtual bool init(SingleEliminationWorld *worldd);
	static SingleElimHud* create(SingleEliminationWorld *worldd);
	virtual void pointsChanged(cocos2d::Vector<Boxx*> *orderedByPointsBoxes);
	virtual void gameIsOver();

};
#endif
