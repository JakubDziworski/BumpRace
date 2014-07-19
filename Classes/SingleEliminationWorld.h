#ifndef  __SINGLE_ELIM_H__
#define  __SINGLE_ELIM_H__
#include "cocos2d.h"
#include "World.h"
class SingleEliminationWorld : public World
{
private:
	Boxx *followMate;
public:
	static cocos2d::Scene* createScene(int numberOfPlayers, int AILevel);
	static SingleEliminationWorld* create(int numberOfPlayers,  int AILevel);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual bool onTouched(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void customWorldUpdate();
	virtual bool myElimInit(int numberOfPlayers, int AILevel);
	virtual void cameraFollow(float dt);
	virtual void putOnBoxes();
	virtual void checkpointReachedExtended(Boxx *box, int pos);
	virtual void restartLevel();
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first);
	virtual void modifyGate(Chcekpoint *inp);
};
#endif
