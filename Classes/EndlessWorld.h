#ifndef ___ENDLESS_WORLD_H__
#define ___ENDLESS_WORLD_H__
#include "World.h"
class EndlessWorld : public World
{
public:
	static cocos2d::Scene *createScene();
	virtual bool init();
	CREATE_FUNC(EndlessWorld);
	virtual void restartLevel();
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);

};
#endif

