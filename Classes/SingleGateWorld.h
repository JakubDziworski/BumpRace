#ifndef  _CARRER_WORLD_H_
#define  _CARRER_WORLD_H_
#include "cocos2d.h"
#include "World.h"
class SingleGateWorld : public World
{
private:
	Boxx *followMate;
public:
	static cocos2d::Scene* createScene(int numberOfPlayers,int AILevel);
	static SingleGateWorld* create(int numberOfPlayers, int AILevel);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual bool onTouched(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void customWorldUpdate();
	virtual bool myInit(int numberOfPlayers, int AILevel);
	virtual void cameraFollow(float dt);
	virtual void putOnBoxes();
	virtual void rozmiescCheckpointy();
	virtual void checkpointReachedExtended(Boxx *box, int pos);

};
#endif
