#ifndef __BOXX_H__
#define __BOXX_H__
#define DEBUG_COCOS2D 2
#include "cocos2d.h"
#include "external/chipmunk/include/chipmunk/chipmunk.h"
class Boxx : public cocos2d::Sprite
{
private:
	cpBody *myBody;
	float wind;
	float maxVel;
	int physPos;
	std::string ID;
	static void gravityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt);
	void displayDebugInfo();
	void updatePhysPos();
	void updateTransform();
protected:
	int racePos;
	cocos2d::Label *debugL;
	virtual void additionalDebugInfo();
public:
	//COCOS PATTERN//
	bool myInit(const std::string& filename, cpSpace *space);
	static Boxx* create(const std::string& filename, std::string ID, cpSpace *space);
	//GETTERS SETTERS//
	std::string getID() const { return ID; }
	void setID(std::string val) { ID = val; }
	float getWind(){ return wind; }
	int getRacePos() { return racePos; }
	void setRacePos(int val) { racePos = val; }
	int getPhysPos(){ return physPos; }
	void setPhysPosition(int pos){ physPos = pos; }
	//MY FUNCTIONS//
	bool isJumping();
	void setBodyPosition(const cocos2d::Vec2 pos);
	void jump();
	bool isOnFlat();
	void updateBox();
	float getVelocity(){if(myBody != nullptr) return myBody->v.x;}
};
#endif
