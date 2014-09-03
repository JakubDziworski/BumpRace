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
	bool sprawdzajPierwszych;
	bool slowmoTriggered;
	cocos2d::Director *director;
    cocos2d::Vector<Boxx*> *orderedBoxes;
	World *world;
	Boxx* plyr;
	bool isLast=false;
	void checkIfCloseToFirst(Boxx* box);
	void checkIfCloseToLast(Boxx *box);
public:
	void setSprawdzajPlayera(Boxx* plyater){ plyr = plyater; }
	void setSprawdzajPierwszych(bool inp);
	void enableSlowmo();
	void setIsLast(bool val) { this->setColor(cocos2d::Color3B(237, 28, 36)); isLast = val; }
	void zwolnij(float dt);
	void tick(float dt);
	static Chcekpoint *create(World *worldd, cocos2d::Vector<Boxx*> *boxess,std::string imagefileName);
	bool init(World *worldd, cocos2d::Vector<Boxx*> *boxess, std::string imagefileName);
	void triggerFirstVisualEffects(Boxx *box);
};

#endif /* defined(__BumpRaceAndro__Checkpoint__) */