#ifndef __POWERUP___
#define __POWERUP___
#include "cocos2d.h"
class Boxx;
class PowerUp : public cocos2d::Sprite
{
public:
	enum class PowerUpType
	{
		SPEED,
		GHOST,
		NONE
	};
	static PowerUp *create(cocos2d::Vector<Boxx*> *boxess);
	bool InitPowerUp(cocos2d::Vector<Boxx*> *boxess);
private:
	bool active;
	PowerUpType pwrupType;
	cocos2d::Vector<Boxx*> *boxez;
	void tick(float dt);
};
#endif