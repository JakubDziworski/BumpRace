#ifndef __GAMETEST_SCENE_H__
#define __GAMETEST_SCENE_H__
#include "cocos2d.h"
#include "Boxx.h"
#include "external/chipmunk/include/chipmunk/chipmunk.h"
class Chcekpoint;
class Hud;
class World :public cocos2d::Layer
{
private:
	//**regular stuff**//
	float timee;
	float angle;
	//**chipmunk stuff**//
	cocos2d::Sprite *bgImg;
	cpBody *floorBody;
	Hud *hud;
	//**myStuff**//
	cocos2d::Vector<Boxx*> physPosOrderedOpponentz;
	//**functions**//
	void s_putOnPlayers();
	void checkPosition(float dt);
	void changeGravity();
	void createFloor();
	void tick(float dt);
	void createBackground();
	static bool posSortingFun(Boxx* a, Boxx* b);
	static bool physPosSortingFun(Boxx *a, Boxx *b);
	static bool scoreSortingFun(Boxx *a, Boxx *b);
protected:
	int aiSmart;
	int gatesNumber;
	int remainingGates;
	cpSpace *gravitySpace;
	int boxesNumber;
	float screenRatio;
	Boxx *player;
	cocos2d::Vector<Boxx*> opponentz;
	cocos2d::Vector<Boxx*> orderedOpponents;
	cocos2d::Vector<Boxx*> orderedOppByScore;
	Boxx *followMate;
	//**cocos stuff**//
	cpShape *floor;
	Node *rotationLayer;
	Node *moveLayer;
	Node *scaleeLayer;
	cocos2d::Point srodek;
	float paralexFactor;
	float posX;	//pozycja x layera patrzacego na player
	float posY; //pozycja y layera patrzacego na playera
	void floorspritefollow(){}
	virtual void s_cameraFollow(float dt);
	virtual bool s_onTouched(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void s_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void customWorldUpdate(){}
public:
	Boxx * getPlayer() const { return player; }
	void tintToBlack();
	int getRemainingGates() const { return remainingGates; }
	virtual void restartLevel() = 0;
	cocos2d::Vector<Boxx*> *getBoxes(){ return &opponentz; }
	cocos2d::Vector<Boxx*> *getSortedBoxesByScore();
	int getBoxesNumber() const { return boxesNumber; }
	void setBoxesNumber(int val) { boxesNumber = val; }
	virtual void checkpointReachedBase(Boxx *box, int pos);
	virtual void checkpointReachedExtended(Boxx *box, int pos){}
	virtual bool myInit(int numberOfPlayers,int gatess);
	virtual bool myInitWithAI(int numberOfPlayers, int gatess, int aiSmartness);
	virtual void rozmiescCheckpointy();
	bool nodeOutOfWindow(cocos2d::Node *node);
	virtual void pauseGame();
	virtual void resumeGame();
	virtual void modifyGate(Chcekpoint *chkpt){}
	Boxx* getOstaniActive();
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first){}
	Hud* getHud();
};
#endif // __GAMETEST_SCENE_H__
