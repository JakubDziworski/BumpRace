#ifndef __GAMETEST_SCENE_H__
#define __GAMETEST_SCENE_H__
#include "cocos2d.h"
#include "Boxx.h"
#include "external/chipmunk/include/chipmunk/chipmunk.h"
#include "extensions/cocos-ext.h"
#include "Player.h"
class Chcekpoint;
class Hud;
class AIOpponent;
class World :public cocos2d::Layer
{
private:
	//**regular stuff**//
	bool gameOver;
	float timee;
	float angle;
	//**chipmunk stuff**//
	//**myStuff**//
	//**functions**//
	void checkPosition(float dt);
	void changeGravity();
	void createFloor();
	void tick(float dt);
	virtual void createBackground();
	static bool posSortingFun(Boxx* a, Boxx* b);
	static bool physPosSortingFun(Boxx *a, Boxx *b);
	static bool scoreSortingFun(Boxx *a, Boxx *b);
	std::function<void()> cameraFollowFunction;
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
protected:
	float dlugoscDrzewekMalych=0;
	float dlugoscDrzewekDuzych=0;
	float bgOffset = 0;
	float wylosowane = 0;
	cocos2d::Sprite *lastChmurka;
	cocos2d::ParallaxNode *cloudsNode = nullptr;
	Hud *hud;
	float odstep = 0.0f; //odstep miedzy dzwiekami zdrzenia;
	const float minOdstep = 0.1f;
	int carrerLevel = 0;
	cocos2d::extension::PhysicsSprite *flatsprite;
	cocos2d::Sprite *bgImg;
	cpBody *floorBody;
	int aiSmart;
	bool multiplayerEnabled;
	int gatesNumber;
	int remainingGates;
	cpSpace *gravitySpace;
	int boxesNumber;
	int playersNumber;
	float screenRatio;
	Player *player;
	cocos2d::Vector<Player*> players;
	cocos2d::Vector<Boxx*> opponentz;
	cocos2d::Vector<Boxx*> physPosOrderedOpponentz;
	cocos2d::Vector<Boxx*> orderedOpponents;
	cocos2d::Vector<Boxx*> orderedOppByScore;
	Boxx *followMate;
	//**cocos stuff**//
	cpShape *floor;
	Node *rotationLayer;
	Node *moveLayer;
	Node *scaleeLayer;
	float paralexFactor;
	float posX;	//pozycja x layera patrzacego na player
	float posY; //pozycja y layera patrzacego na playera
	float DPIscaleFactor;
	bool paused=false;
	bool started = false;
	void floorspritefollow(){}
	virtual void s_cameraFollow();
	void s_putOnPlayers(Player* playerr);
	virtual bool s_onTouched(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void s_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void m_cameraFollow();
	void m_putOnPlayers(cocos2d::Vector<Player*> players);
	virtual void m_onTouched(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void m_onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void customWorldUpdate(){}
	void lateInit();
	void replaceSceneGenereal(cocos2d::Scene *scene,World *world);
    virtual void replaceSceneAdditional(cocos2d::Scene *scene,World *world){};
public:
	std::function<void()> tapToContinueTapped;
	bool hasStarted() const { return started; }
	bool isPaused() const { return paused; }
	bool isGameOver() const { return gameOver; }
	cpShape *getFloor() { return floor; }
	bool isMultiplayer(){ return multiplayerEnabled; }
	void setCarrierLevel(int val){ carrerLevel = val; }
	int getCarrerLevel(){ return carrerLevel; }
	cpSpace * getGravitySpace() const { return gravitySpace; }
	void setMultiplayer(cocos2d::Vector<Player*> players);
	void setSinglePlayer(Player* player);
	Boxx * getPlayer() const { return player; }
	void tintToBlack();
	int getRemainingGates() const { return remainingGates; }
	virtual void restartLevel() = 0;
	cocos2d::Vector<Player*> *getPlayers() { return &players; }
	cocos2d::Vector<Boxx*> *getBoxes(){ return &opponentz; }
	cocos2d::Vector<Boxx*> *getOrderedBoxes(){ return &orderedOpponents; }
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
	Boxx* getPrzedOstaniActive();
	virtual void shouldEnableSlowmo(Chcekpoint *chkpt, bool first){}
	Hud* getHud();
	void setHud(Hud *hudd){ hud = hudd; }
	virtual void gameIsOver(bool win);
	void calculateSredniaPredkoscDoDzwieku();
	void startBoxPointer();
	//onExit
	void onExit();
	//PHYSICS ACTIONS
	static void boxesCollided(cpArbiter *arb, cpSpace *space, void *unused);
	static void boxFeltDown(cpArbiter *arb, cpSpace *space, void *unused);
	//generation;
	void generateClouds();
	void generateDrzewka();
};
#endif // __GAMETEST_SCENE_H__
