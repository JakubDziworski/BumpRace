#ifndef  __SINGLE_ELIM_H__
#define  __SINGLE_ELIM_H__
#include "cocos2d.h"
#include "World.h"
class SingleElimHud;
class SingleEliminationWorld : public World
{
private:
	SingleElimHud *hud;
public:
	static cocos2d::Scene* createScene(int numberOfPlayers, int AILevel);
	static SingleEliminationWorld* create(int numberOfPlayers,  int AILevel);
	virtual void customWorldUpdate();
	virtual bool myElimInit(int numberOfPlayers, int AILevel);
	virtual void putOnBoxes();
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void restartLevel();
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);
	virtual void modifyGate(Chcekpoint *inp);
};
#endif
