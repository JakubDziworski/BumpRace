#ifndef  _CARRER_WORLD_H_
#define  _CARRER_WORLD_H_
#include "cocos2d.h"
#include "World.h"
class CarrerWorld : public World
{
private:
	Boxx *followMate;
public:
	static cocos2d::Scene* createScene(int numberOfPlayers);
	static CarrerWorld* create(int numberOfPlayers);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual bool onTouched(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void customWorldUpdate();
	virtual bool myInit(int numberOfPlayers);
	virtual void cameraFollow(float dt);
	virtual void putOnBoxes();


};
#endif
