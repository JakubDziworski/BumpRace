#ifndef  _CARRER_WORLD_H_
#define  _CARRER_WORLD_H_
#include "cocos2d.h"
#include "World.h"
class SingleGateWorld : public World
{
public:
	static cocos2d::Scene* createScene(int numberOfPlayers, int gatess, int AILevel);
	static SingleGateWorld* create(int numberOfPlayers, int gatess, int AILevel);
	virtual void customWorldUpdate();
	virtual bool myInit(int numberOfPlayers, int gatess, int AILevel);
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void restartLevel();
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);

};
#endif
