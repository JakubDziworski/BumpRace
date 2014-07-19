#ifndef __BumpRaceAndro__Checkpoint__
#define __BumpRaceAndro__Checkpoint__
#include "cocos2d.h"
#include "Boxx.h"
#include "World.h"
class Chcekpoint : public cocos2d::Sprite
{
private:
	int actualpos;
	float timee;
	bool pierwszyZlapal;
	bool slowmoTriggered;
	cocos2d::Director *director;
	cocos2d::Vector<Boxx*> *orderedBoxes;
	World *world;
	bool isLast;
public:
	void setIsLast(bool val) { isLast = val; }
	void przyspiesz(float dt);
	void zwolnij(float dt);
	void zwolnijNaOstanim(float dt);
	void tick(float dt);
	static Chcekpoint *create(World *worldd, cocos2d::Vector<Boxx*> *boxess,std::string imagefileName);
	bool init(World *worldd, cocos2d::Vector<Boxx*> *boxess, std::string imagefileName);
};

#endif /* defined(__BumpRaceAndro__Checkpoint__) */