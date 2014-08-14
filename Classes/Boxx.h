#ifndef __BOXX_H__
#define __BOXX_H__
#include "cocos2d.h"
#include "external/chipmunk/include/chipmunk/chipmunk.h"
#include "PowerUp.h"
class Boxx : public cocos2d::Sprite
{
private:
	PowerUp::PowerUpType pwrupType;
	int points;
	cpShape *shapes[3];
	cpBody *myBody;
	bool deactivated;
	float wind;
	std::string fileName;
	float maxVel;
	int physPos;
	cocos2d::Color3B boxColor;
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
	cocos2d::Color3B getBoxColor() const { return boxColor; }
	bool myInit(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr);
	static Boxx* create(const std::string& filename, std::string ID, cpSpace *space, cocos2d::Color3B boxColorr = cocos2d::Color3B::WHITE);
	//GETTERS SETTERS//
	std::string getFileName() const { return fileName; }
	bool isDeactivated() const { return deactivated; }
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
	float getVelocity(){ if (myBody != nullptr) return  myBody->v.x; }
	void addPoint();
	int getScore(){ return points; }
	void deactivate();
	void collectedPowerUp(PowerUp::PowerUpType pwruptype);
	void activatePowerUp();
};
#endif
