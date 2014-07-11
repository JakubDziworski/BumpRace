#ifndef __GAMETEST_SCENE_H__
#define __GAMETEST_SCENE_H__
#include "cocos2d.h"
#include "Boxx.h"
#include "external/chipmunk/include/chipmunk/chipmunk.h"
class World :
	public cocos2d::Layer
{
private:
	//**regular stuff**//
	float timee;
	float angle;
	//**chipmunk stuff**//
	cpBody *floorBody;
	cpShape *floor;
	//**myStuff**//
	cocos2d::Vector<Boxx*> physPosOrderedOpponentz;
	//**functions**//
	void putOnPlayers();
	void checkPosition(float dt);
	void changeGravity();
	void createFloor();
	void tick(float dt);
	static bool posSortingFun(Boxx* a, Boxx* b);
	static bool physPosSortingFun(Boxx *a, Boxx *b);
protected:
	cpSpace *gravitySpace;
	int boxesNumber;
	Boxx *player;
	cocos2d::Vector<Boxx*> opponentz;
	cocos2d::Vector<Boxx*> orderedOpponents;
	//**cocos stuff**//
	Node *rotationLayer;
	Node *scaleLayer;
	cocos2d::Point srodek;
	float posX;	//pozycja x layera patrzacego na player
	float posY; //pozycja y layera patrzacego na playera
	virtual void putOnBoxes();
	virtual void cameraFollow(float dt)=0;
	virtual bool onTouched(cocos2d::Touch* touch, cocos2d::Event* event)=0;
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)=0;
	virtual void customWorldUpdate()=0;
public:
	int getBoxesNumber() const { return boxesNumber; }
	void setBoxesNumber(int val) { boxesNumber = val; }
	virtual void checkpointReached(Boxx *box, int pos);
	virtual bool myInit(int numberOfPlayers);
	virtual void rozmiescCheckpointy();
};
#endif // __GAMETEST_SCENE_H__
